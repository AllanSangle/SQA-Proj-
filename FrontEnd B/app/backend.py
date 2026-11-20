# Backend Prototype
# The backend reads the input files and transaction files to apply changes from the transaction to the input files, while logging errors
# The frontend runs "python backend.py currentaccounts.txt availablegames.txt gamescollection.txt mergedtransactions.atf" after each session
# No changes to how the frontend is ran (still the same as specified in README.md)

import sys
from pathlib import Path

# Handles file operations such as reading, writing, and checking for bad input
class FileHandler:
    def __init__(self):
        self.fatal_errors = [] # list to store fatal errors

    # reads through the contents within a file to get information from input files
    def readFile(self, path):
        # opens the file and attempts to read it
        try:
            return Path(path).read_text(encoding="utf-8").splitlines()
        except OSError as inputError:
            # logs a fatal error if the file is unable to be read
            self.addFatalErrors(f"unable to read file \"{path}\": {inputError}")
            return None

    # writes information to a file to update input files
    def writeFile(self, path, lines):
        # opens the file and attempts to write to it
        try:
            Path(path).write_text("\n".join(lines) + "\n", encoding="utf-8")
            return True
        except OSError as inputError:
            # logs a fatal error if the file is unable to be written to
            self.addFatalErrors(f"unable to write to file \"{path}\": {inputError}")
            return False

    # checks if there are bad input files that failed to read or had invalid input contents
    def isBadInput(self, path, lines):
        if lines is None:
            return True
        for number, line in enumerate(lines, start=1):
            if not isinstance(line, str):
                self.addFatalErrors(f"Invalid input in \"{path}\" (Row {number})")
                return True
        return False

    # handles adding fatal errors to the list
    def addFatalErrors(self, fatalError):
        self.fatal_errors.append(f"ERROR: (FATAL) {fatalError}")

    # displays fatal errors in the terminal when they occur
    def displayFatalError(self):
        for error in self.fatal_errors:
            print(error)




# Logs failed constraint errors within the program
class FailedConstraintLog:
    def __init__(self):
        self.constraint_errors = [] # stores constraint errors
    
    # check if a user exists constraint 
    def userExists(self, username, accounts):
        return username in accounts

    # check if a balance is negative constraint 
    def isNegativeBalance(self, username, accounts):
        return username in accounts and accounts[username]["credit"] < 0

    # adds constraint errors to the list
    def addConstraintErrors(self, constraintError):
        self.constraint_errors.append(f"ERROR: {constraintError}")

    # displays constraint errors in the terminal
    def displayConstraintErrors(self):
        for error in self.constraint_errors:
            print(error)



# handles the daily transaction files functions
class DailyTransactionFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler

    # adds a transaction to the daily transaction files
    def addTransaction(self, transaction):
        # opens the file and attempts to write to it
        try:
            with open(self.path, "a", encoding="utf-8") as file:
                file.write(transaction.rstrip("\n") + "\n")
            return True
        except OSError as inputError:
            # logs a fatal error if the daily transaction file can't be read since this would be bad input for the merged transaction file
            self.file_handler.addFatalErrors(f"unable to add transaction to transaction file \"{self.path}\": {inputError}")
            return False

    # empties the transaction file
    def clearTransactions(self):
        return self.file_handler.writeFile(self.path, [])




# Handles loading and modifying user accounts and their information
class UserAccountsFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler
        self.accounts = {} # stores accounts and their information from the accounts file and to be added to the file

    # Loads each account from the user accounts file and gets their username, type, and credit
    def load(self):
        # reads the user accounts file
        lines = self.file_handler.readFile(self.path)

        # checks if the user accounts file is valid
        if self.file_handler.isBadInput(self.path, lines):
            return False

        # empties any previous accounts to reload the accounts file and avoid duplicates
        self.accounts.clear()

        # goes through each user entry
        for line in lines:
            # stop at the end, when there aren't users left to read
            if line.strip() == "END":
                break
            # checks for an invalid user entry format based on length
            if len(line) != 28:
                self.file_handler.addFatalErrors(f"Invalid account input in \"{self.path}\": {line!r}")
                return False
            # gets each field in the entry
            username = line[:15].strip()
            user_type = line[16:18].strip()
            credit_text = line[-9:]
            # checks if the credit balance is a valid type, and logs a fatal error if not
            try:
                credit = float(credit_text)
            except ValueError:
                self.file_handler.addFatalErrors(f"Invalid credit value input type in \"{self.path}\": {line!r}")
                return False
            # adds the account and its information to the dictionary
            self.accounts[username] = {"type": user_type, "credit": credit}
        return True

    # Adds a user to the accounts dictionary to add it to the accounts file
    def addUser(self, username, user_type):
        self.accounts[username] = {"type": user_type, "credit": 0.0}

    # Deletes a user from the accounts dictionary to delete it from the accounts file
    def deleteUser(self, username):
        self.accounts.pop(username, None)

    # Updates a user's balance in the accounts dictionary to update it in the accounts file
    def updateUserCredits(self, username, credits):
        if username in self.accounts:
            self.accounts[username]["credit"] = round(credits, 2)

    # Saves the final updated account information to the user accounts file
    def saveAccountInfo(self):
        lines = []
        for username in sorted(self.accounts):
            account = self.accounts[username]
            lines.append(f"{username:<15} {account['type']:<2} {account['credit']:09.2f}")
        lines.append("END") # adds the final entry to indicate there are no more users
        return self.file_handler.writeFile(self.path, lines)



# Contains and updates available game information
class AvailableGamesFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler
        self.games = {} # stores the games in the file and to be added to the file

    # Loads the available games from the available games file and gets each game’s name, seller and price
    def load(self):
        # reads the available games file
        lines = self.file_handler.readFile(self.path)

        # checks if the available games file is valid
        if self.file_handler.isBadInput(self.path, lines):
            return False

        # empties any previous games to reload the games file and avoid duplicates
        self.games.clear()

        # goes through each game entry
        for line in lines:
            # stop at the end, when there aren't games left to read
            if line.strip() == "END":
                break
            # checks for an invalid game entry format based on length
            if len(line) != 49:
                self.file_handler.addFatalErrors(f"Invalid game input in \"{self.path}\": {line!r}")
                return False
            # gets each field in the entry
            price_start = len(line) - 6
            seller_start = price_start - 1 - 15
            name = line[:seller_start].strip()
            seller = line[seller_start:seller_start + 15].strip()
            # checks if the price is a valid type, and logs a fatal error if not
            try:
                price = float(line[-6:])
            except ValueError:
                self.file_handler.addFatalErrors(f"Invalid game price input in \"{self.path}\": {line!r}")
                return False
            # adds the game and its information (seller and price) to the dictionary
            self.games[name] = {"seller": seller, "price": price}
        return True

    # Adds a game to the games dictionary to add it to the available games file
    def addGame(self, game, seller, price):
        self.games[game] = {"seller": seller, "price": round(price, 2)}

    # Deletes a game from the games dictionary to remove it from the available games file
    def deleteAvaliableGames(self, game):
        self.games.pop(game, None)

    # Saves the final updated available games information to the available games file
    def saveGameInfo(self):
        lines = []
        for name in sorted(self.games):
            item = self.games[name]
            lines.append(f"{name:<{26}} {item['seller']:<{15}} {item['price']:06.2f}")
        lines.append("END")
        return self.file_handler.writeFile(self.path, lines)




# Contains and updates information about games owned by users
class GameCollectionFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler
        self.collection = set() # stores owned games with the owner and seller

    # Loads the owned games from the game collection file and gets each owner of a game, the owned game, and the seller
    def load(self):
        # reads the game collection file
        lines = self.file_handler.readFile(self.path)
        # checks if the game collection file is valid
        if self.file_handler.isBadInput(self.path, lines):
            return False

        # empties any previous collections to reload the game collection file and avoid duplicates
        self.collection.clear()
        for line in lines:
            # stop at the end, when there aren't game collections left to read
            if line.strip() == "END":
                break
            # checks for an invalid game collection entry format based on length
            if len(line) != 42:
                self.file_handler.addFatalErrors(f"Invalid game collection input in \"{self.path}\": {line!r}")
                return False
            # gets each field in the entry
            game = line[:26].strip()
            owner = line[27:].strip()
            self.collection.add((game, owner))
        return True

    # Adds a collection to the collections dictionary to add it to the game collection file
    def addOwnedGame(self, username, game):
        self.collection.add((game, username))

    # Checks if a user already owns a game
    def ownsGame(self, username, game):
        return (game, username) in self.collection

    # Removes a collection to the collections dictionary to remove it from the game collection file
    def removeUserGames(self, username):
        self.collection = {entry for entry in self.collection if entry[1] != username}

    # Saves the final updated available games information to the available games file
    def saveCollectionInfo(self):
        lines = [
            f"{game:<{26}} {owner:<{15}}"
            for game, owner in sorted(self.collection)
        ]
        lines.append("END")
        return self.file_handler.writeFile(self.path, lines)




# Merges all the daily transaction files into one file to produce updated input files
class MergedDailyTransactions:
    def __init__(self, path, file_handler, accounts, games, collection, log):
        self.path = path
        self.file_handler = file_handler
        self.accounts = accounts
        self.games = games
        self.collection = collection
        self.log = log
        self.transactions = [] # stores transactiosn

    # gets every transaction
    def getAllTransactions(self):
        # read file and check if valid input
        lines = self.file_handler.readFile(self.path)
        if self.file_handler.isBadInput(self.path, lines):
            return False
        self.transactions = lines
        return True


    # Breaks down the fields in the transaction records and determines its code, and any user’s games, or credits involved
    @staticmethod
    def _fields(line):
        code = line[:2] # gets the code a transaction
        # checks if transaction entry is invalid
        if len(line) < 3 or line[2] != " ":
            raise ValueError("Invalid transaction code format")

        # checks if transaction input is valid and gets the fields (username, type, credit) for transactions with similar fields
        if code in ("00", "01", "02", "06"):
            if len(line) != 31:
                raise ValueError("Invalid user update transaction length")
            username = line[3:18].strip()
            user_type = line[19:21].strip()
            credit_amount = float(line[-9:])
            return code, username, user_type, credit_amount

        # checks if transaction input is valid and gets the fields (buyer, seller, refund amount) for refund transactions
        if code == "05":
            if len(line) != 44:
                raise ValueError("Invalid refund transaction length")
            buyer = line[3:18].strip()
            seller = line[19:34].strip()
            refund_amount = float(line[-9:])
            return code, buyer, seller, refund_amount

        # checks if transaction input is valid and gets the fields (game, seller, price) for sell transactions
        if code == "03":
            if len(line) != 52:
                raise ValueError("Invalid sell transaction length")
            game = line[3:29].strip()
            seller = line[30:45].strip()
            price = float(line[-6:])
            return code, game, seller, price

        # checks if transaction input is valid and gets the fields (game, seller, buyer, price) for buy transactions
        if code == "04":
            if len(line) != 68:
                raise ValueError("Invalid buy transaction length")
            game = line[3:29].strip()
            seller = line[30:45].strip()
            buyer = line[46:61].strip()
            price = float(line[-6:])
            return code, game, seller, buyer, price

        raise ValueError(f"Invalid transaction code \"{code}\"")

    # Applies changes for user accounts using transaction information to apply the changes the user accounts file
    def addUserChanges(self, transaction):
        code = transaction[0]

        # gets the fields from the transaction to add the correct new user information to the file 
        if code == "01":
            username, user_type, amount = transaction[1], transaction[2], transaction[3]
            # error creating user if it already exists
            if self.log.userExists(username, self.accounts.accounts):
                self.log.addConstraintErrors(f"Creating user that already exists ({self._current_line})")
                return
            # adds user
            self.accounts.addUser(username, user_type)

        # gets the fields from the transaction to delete the correct user from the file 
        elif code == "02":
            username = transaction[1]
            self.accounts.deleteUser(username)
            self.games.games = {
                name: item for name, item in self.games.games.items()
                if item["seller"] != username
            }
        
        # gets the fields from the transaction to make the correct changes to the correct user's credit 
        elif code == "06":
            username, user_type, amount = transaction[1], transaction[2], float(transaction[3])
            if username not in self.accounts.accounts:
                self.log.addConstraintErrors(f"Adding credit to a non-existent user ({self._current_line})")
                return
            self.accounts.accounts[username]["credit"] += amount

    # Applies changes for the available games using transaction information to apply the changes to the available games file
    def addAvailableGames(self, transaction):
        code = transaction[0]
        # gets the fields from the transaction to add a new game with the correct name, seller, and price
        if code == "03":
            _, game, seller, price = transaction
            self.games.addGame(game, seller, price)
        # gets the fields from the transaction to delete a seller's games if their account was deleted
        elif code == "02":
            username = transaction[1]
            self.games.games = {
                name: item for name, item in self.games.games.items()
                if item["seller"] != username
            }

    # Uses transaction information to apply the required changes to owned games
    def addCollectionChanges(self, transaction):
        if transaction[0] == "04":
            _, game, seller, buyer, price = transaction
            self.collection.addOwnedGame(buyer, game)

    # Uses transaction information to update user credit information from refund while checking  if buyer and seller users exist for the constraint
    def addRefundChanges(self, transaction):
        if transaction[0] != "05":
            return
        _, buyer, seller, amount = transaction
        if buyer not in self.accounts.accounts or seller not in self.accounts.accounts:
            self.log.addConstraintErrors(f"Refund refers to a user that does not exist ({self._current_line})")
            return
        self.accounts.accounts[seller]["credit"] -= float(amount)
        self.accounts.accounts[buyer]["credit"] += float(amount)

    # Runs the procedure of the merged transaction file, by reading through transactions and applying its required changes
    def run(self):
        # gets all transaactions
        if not self.getAllTransactions():
            return False

        # goes through each transaction and checks its code to apply the correct changes to the correct users
        for line in self.transactions:
            if not line.strip():
                continue
            # input files aren't changed on logout transactions
            if line[:2] == "00":
                continue
            
            self._current_line = line
            try:
                # gets transaction and its code
                transaction = self._fields(line)
                code = transaction[0]

                # if the transaction requires changes to the accounts file, then apply them 
                if code in ("01", "02", "06"):
                    self.addUserChanges(transaction)
                # if the transaction is a refund, then apply the credit change to the user 
                elif code == "05":
                    self.addRefundChanges(transaction)
                # if the transaction is sell, then make the correct credit changes to the users balance
                elif code == "03":
                    _, game, seller, price = transaction
                    # error if the seller isn't an existing user
                    if seller not in self.accounts.accounts:
                        self.log.addConstraintErrors(f"Seller isn't an existing user (transaction: {line})")
                        continue
                    self.addAvailableGames(transaction)
                # if the transaction is a buy transaction, then apply the changes to be made in the game collection file 
                elif code == "04":
                    _, game, seller, buyer, price = transaction
                    # error if the game being purchased doesn't exist
                    if game not in self.games.games:
                        self.log.addConstraintErrors(f"Game being bought doesn't exist (transaction: {line})")
                        continue
                    # error if user is purchasing a game they already own
                    if self.collection.ownsGame(buyer, game):
                        self.log.addConstraintErrors(f"Buyer buying a game that they already own (transaction: {line}")
                        continue
                    self.addCollectionChanges(transaction)
            # fatal error if invalid input in transaction
            except (ValueError, IndexError) as inputError:
                self.file_handler.addFatalErrors(f"Invalid transaction in \"{self.path}\": {line!r} ({inputError})")
                return False

        return True


# runs through the merged transaction and makes changes to the input files based on the transactions 
def main():
    base_dir = Path(__file__).resolve().parent
    # sets the feault path of the input files if not specified when running
    default_paths = (
        str(base_dir / "currentaccounts.txt"),
        str(base_dir / "availablegames.txt"),
        str(base_dir / "gamescollection.txt"),
        str(base_dir / "mergedtransactions.atf"),
    )

    # sets the path to the input file based on the specified arguments in the terminal, or uses the default paths if unspecified
    accounts_path, games_path, collection_path, merged_path = sys.argv[1:5] if len(sys.argv) >= 5 else default_paths

    # sets the classes with the input files
    files = FileHandler()
    log = FailedConstraintLog()
    accounts = UserAccountsFile(accounts_path, files)
    available_games = AvailableGamesFile(games_path, files)
    game_collection = GameCollectionFile(collection_path, files)

    # fatal error if input files fail to load
    if not accounts.load() or not available_games.load() or not game_collection.load():
        files.displayFatalError()
        return 1

    # sets the merged faily transactions with the correct input files to read and make changes to
    merged = MergedDailyTransactions(merged_path, files, accounts, available_games, game_collection, log)

    # fatal error if the merged file procedure can't be done
    if not merged.run():
        files.displayFatalError()
        log.displayConstraintErrors()
        return 1

    # aplies changes to the input files, and gives an error if it fails
    if not accounts.saveAccountInfo() or not available_games.saveGameInfo() or not game_collection.saveCollectionInfo():
        files.displayFatalError()
        return 1

    # display constraint and fatal errors
    log.displayConstraintErrors()
    files.displayFatalError()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
