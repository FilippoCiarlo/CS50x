import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

BIRTHDAY_DATA = {"name": "Name", "month": "Month", "day": "Day"}


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Add the user's entry into the database

        # Validate submission
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        if int(month) in range(1, 13) and int(day) in range(1, 32):
            db.execute(
                "INSERT INTO birthdays (name,month,day) VALUES (?,?,?)",
                name,
                month,
                day,
            )
        else:
            return render_template("failure.html")

        # Show birthdays
        return redirect("/")

    else:
        # Display the entries in the database on index.html
        birthdays = db.execute("SELECT name,month,day FROM birthdays")
        return render_template(
            "index.html", birthdays=birthdays, birthday_data=BIRTHDAY_DATA
        )
