def main():
    menu = {
        "Baja Taco": 4.00,
        "Burrito": 7.50,
        "Bowl": 8.50,
        "Nachos": 11.00,
        "Quesadilla": 8.50,
        "Super Burrito": 8.50,
        "Super Quesadilla": 9.50,
        "Taco": 3.00,
        "Tortilla Salad": 8.00,
    }

    total = 0
    while True:
        try:
            # Ask for an item
            item = input("Item: ").title()
            # Check the presence of the item in the menu
            if item in menu:
                # Compute the total
                total += menu[item]
                print(f"Total: ${total:.2f}")
        except EOFError:
            print()
            break


main()
