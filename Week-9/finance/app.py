import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


# Query database for cash
def get_cash(id, usd_format=False):
    # Query database for user's cash
    row = db.execute(
        "SELECT cash FROM users WHERE id = ? ",
        id,
    )
    cash = float(row[0]["cash"])

    # Format money output
    if usd_format:
        return usd(cash)
    else:
        return float(cash)


# Retrive quote info
def get_quote_info(symbol, name=False, usd_format=False):
    # Lookup for the symbol
    quote = lookup(symbol)

    # Ensure the symbol exist
    if not quote:
        return None
    else:
        quote_info = dict()
        quote_info["price"] = float(quote["price"])
        quote_info["symbol"] = quote["symbol"]

        if name:
            quote_info["name"] = quote["name"]

        # Format money output
        if usd_format:
            quote_info["price"] = usd(quote_info["price"])

        return quote_info


# Attempts to convert the input string to an integer.
def try_parse_int(string_value):
    try:
        int_value = int(string_value)
        return int_value
    except ValueError:
        return None


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Retrieve user id
    user_id = session["user_id"]

    # Query database for portfolio
    shares = db.execute(
        "SELECT symbol, shares FROM portfolio WHERE user_id = ?",
        user_id,
    )

    # Retrive and Compute portfolio info
    total = get_cash(user_id)
    for share in shares:
        quote = get_quote_info(share["symbol"], name=True)
        share["name"] = quote["name"]
        share["price"] = usd(quote["price"])
        shares_value = float(quote["price"]) * int(share["shares"])
        share["total"] = usd(shares_value)
        total += shares_value
        print(share)

    # Display the home page
    return render_template(
        "index.html",
        shares=shares,
        cash=get_cash(user_id, usd_format=True),
        total=usd(total),
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Retrieve user id
    user_id = session["user_id"]

    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        else:
            symbol = request.form.get("symbol")
            # Check if the symbol is valid
            quote = get_quote_info(symbol)
            if quote is None:
                return apology("invalid symbol", 400)
            else:
                symbol = quote["symbol"]

        # Ensure number of shares was submitted
        if not request.form.get("shares"):
            return apology("missing shares", 403)
        else:
            # Check if the number of shares is valid
            shares = try_parse_int(request.form.get("shares"))
            if shares is None or shares <= 0:
                return apology("inavlid number of shares", 400)

        # Retrieve cash on the account
        cash = get_cash(user_id)

        # Check the feasibility of the operation
        cost = shares * quote["price"]
        if cost > cash:
            return apology("can't afford", 400)
        else:
            cash -= cost

        # Query database for shares
        rows = db.execute(
            "SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?",
            user_id,
            symbol,
        )

        # Update the Portfolio
        if len(rows) == 0:
            db.execute(
                "INSERT INTO portfolio (user_id,symbol,shares) VALUES (?,?,?)",
                user_id,
                symbol,
                shares,
            )
        else:
            updated_shares = int(rows[0]["shares"]) + shares
            db.execute(
                "UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                updated_shares,
                user_id,
                symbol,
            )

        # Update transactions book
        db.execute(
            "INSERT INTO transactions (user_id,symbol,shares,price) VALUES (?,?,?,?)",
            user_id,
            symbol,
            shares,
            quote["price"],
        )

        # Update cash availability  on the account
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            cash,
            user_id,
        )

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Retrieve user id
    user_id = session["user_id"]

    # Query database for stransactions hisotry
    transactions = db.execute(
        "SELECT * FROM  transactions WHERE user_id = ? ORDER BY date DESC;",
        user_id,
    )
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        else:
            symbol = request.form.get("symbol")

        # Retrieve quote info
        quote = get_quote_info(symbol, name=True, usd_format=True)
        if quote is None:
            return apology("invalid symbol", 400)

        # Redirect user to quote info page
        return render_template("quoted.html", quote=quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation password was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation password", 400)

        # Retrive submitted data
        else:
            username = request.form.get("username")
            password = request.form.get("password")
            confirmation = request.form.get("confirmation")

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username is not already in use and the two password matches
        if len(rows) != 0:
            return apology("invalid username", 400)
        elif password != confirmation:
            return apology("invalid confirmation password", 400)

        # Generate password hash
        hash = generate_password_hash(request.form.get("password"))

        # Add the new user into the database
        db.execute("INSERT INTO users (username,hash) VALUES (?,?)", username, hash)

        # Redirect user to login form
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Retirve user id
    user_id = session["user_id"]

    # Query database for portfolio
    shares = db.execute(
        "SELECT symbol FROM  portfolio WHERE user_id = ?",
        user_id,
    )

    if request.method == "POST":
        # Ensure share was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        else:
            symbol = request.form.get("symbol")
            # Check if the symbol is valid
            quote = get_quote_info(symbol)
            if quote is None:
                return apology("invalid symbol", 400)
            else:
                symbol = quote["symbol"]

        # Ensure number of share was submitted
        if not request.form.get("shares"):
            return apology("missing shares", 400)
        else:
            # Check if the number of shares is valid
            shares = try_parse_int(request.form.get("shares"))
            if shares is None or shares <= 0:
                return apology("inavlid number of shares", 400)

        # Query database for number of shares
        row = db.execute(
            "SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?",
            user_id,
            symbol,
        )
        total_shares = int(row[0]["shares"])

        # Check the feasibility of the operation
        if shares > total_shares:
            return apology("too many shares", 400)
        else:
            total_shares -= shares

        # Update the Portfolio
        if total_shares == 0:
            db.execute(
                "DELETE FROM portfolio WHERE user_id = ? AND symbol = ?",
                user_id,
                symbol,
            )
        else:
            db.execute(
                "UPDATE portfolio SET shares = ? WHERE user_id = ? AND symbol = ?",
                total_shares,
                user_id,
                symbol,
            )

        # Update transactions book
        db.execute(
            "INSERT INTO transactions (user_id,symbol,shares,price) VALUES (?,?,?,?)",
            user_id,
            symbol,
            -shares,
            quote["price"],
        )

        # Update cash on the account
        cash = get_cash(user_id)
        received = shares * quote["price"]
        cash += received
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            cash,
            user_id,
        )

        # Redirect user to home page
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", shares=shares)
