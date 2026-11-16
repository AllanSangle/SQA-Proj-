# Backend Prototype
# python backend.py currentaccounts.txt availablegames.txt gamescollection.txt mergedtransactions.atf

import sys
from pathlib import Path

class FileHandler:
    def __init__(self):
        self.fatal_errors = []

    def readFile(self, path):
        try:
            return Path(path).read_text(encoding="utf-8").splitlines()
        except OSError as inputError:
            self.addFatalErrors(f"unable to read file \"{path}\": {inputError}")
            return None

    def writeFile(self, path, lines):
        try:
            Path(path).write_text("\n".join(lines) + "\n", encoding="utf-8")
            return True
        except OSError as inputError:
            self.addFatalErrors(f"unable to write to file \"{path}\": {inputError}")
            return False

    def isBadInput(self, path, lines):
        if lines is None:
            return True
        for number, line in enumerate(lines, start=1):
            if not isinstance(line, str):
                self.addFatalErrors(f"Invalid input in \"{path}\" (Row {number})")
                return True
        return False
    def addFatalErrors(self, fatalError):
        self.fatal_errors.append(f"ERROR: (FATAL) {fatalError}")

    def displayFatalError(self):
        for error in self.fatal_errors:
            print(error)





class FailedConstraintLog:
    def __init__(self):
        self.constraint_errors = []
        self.fatal_errors = []

    def userExists(self, username, accounts):
        return username in accounts

    def isNegativeBalance(self, username, accounts):
        return username in accounts and accounts[username]["credit"] < 0

    def addConstraintErrors(self, constraintError):
        self.constraint_errors.append(f"ERROR: {constraintError}")

    def displayConstraintErrors(self):
        for error in self.constraint_errors:
            print(error)





class DailyTransactionFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler

    def addTransaction(self, transaction):
        try:
            with open(self.path, "a", encoding="utf-8") as file:
                file.write(transaction.rstrip("\n") + "\n")
            return True
        except OSError as inputError:
            self.file_handler.addFatalErrors(f"unable to add transaction to transaction file \"{self.path}\": {inputError}")
            return False

    def clearTransactions(self):
        return self.file_handler.writeFile(self.path, [])





class UserAccountsFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler
        self.accounts = {}

    def load(self):
        lines = self.file_handler.readFile(self.path)
        
        if self.file_handler.isBadInput(self.path, lines):
            return False

        self.accounts.clear()
        for line in lines:
            if line.strip() == "END":
                break
            if len(line) != 28:
                self.file_handler.addFatalErrors(f"Invalid account input in \"{self.path}\": {line!r}")
                return False
            username = line[:15].strip()
            user_type = line[16:18].strip()
            credit_text = line[-9:]
            try:
                credit = float(credit_text)
            except ValueError:
                self.file_handler.addFatalErrors(f"Invalid credit value input type in \"{self.path}\": {line!r}")
                return False
            self.accounts[username] = {"type": user_type, "credit": credit}
        return True

    def addUser(self, username, user_type):
        self.accounts[username] = {"type": user_type, "credit": 0.0}

    def deleteUser(self, username):
        self.accounts.pop(username, None)

    def updateUserCredits(self, username, credits):
        if username in self.accounts:
            self.accounts[username]["credit"] = round(credits, 2)

    def saveAccountInfo(self):
        lines = []
        for username in sorted(self.accounts):
            account = self.accounts[username]
            lines.append(f"{username:<15} {account['type']:<2} {account['credit']:09.2f}")
        lines.append("END")
        return self.file_handler.writeFile(self.path, lines)






class AvailableGamesFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler
        self.games = {}

    def load(self):
        lines = self.file_handler.readFile(self.path)
        if self.file_handler.isBadInput(self.path, lines):
            return False

        self.games.clear()
        for line in lines:
            if line.strip() == "END":
                break
            if len(line) != 49:
                self.file_handler.addFatalErrors(f"Invalid game input in \"{self.path}\": {line!r}")
                return False
            name = line[:26].strip()
            seller = line[16:42].strip()
            try:
                price = float(line[-6:])
            except ValueError:
                self.file_handler.addFatalErrors(f"Invalid game price input in \"{self.path}\": {line!r}")
                return False
            self.games[name] = {"seller": seller, "price": price}
        return True

    def getTransactionChanges(self, transaction):
        return transaction

    def addGame(self, game, seller, price):
        self.games[game] = {"seller": seller, "price": round(price, 2)}

    def deleteAvaliableGames(self, game):
        self.games.pop(game, None)

    def saveGameInfo(self):
        lines = []
        for name in sorted(self.games):
            item = self.games[name]
            lines.append(f"{name:<{26}} {item['seller']:<{15}} {item['price']:06.2f}")
        lines.append("END")
        return self.file_handler.writeFile(self.path, lines)






class GameCollectionFile:
    def __init__(self, path, file_handler):
        self.path = path
        self.file_handler = file_handler
        self.collection = set()

    def load(self):
        lines = self.file_handler.readFile(self.path)
        if self.file_handler.isBadInput(self.path, lines):
            return False

        self.collection.clear()
        for line in lines:
            if line.strip() == "END":
                break
            if len(line) != 42:
                self.file_handler.addFatalErrors(f"Invalid game collection input in \"{self.path}\": {line!r}")
                return False
            game = line[:26].strip()
            owner = line[27:].strip()
            self.collection.add((game, owner))
        return True

    def addOwnedGame(self, username, game):
        self.collection.add((game, username))

    def ownsGame(self, username, game):
        return (game, username) in self.collection

    def removeUserGames(self, username):
        self.collection = {entry for entry in self.collection if entry[1] != username}

    def saveCollectionInfo(self):
        lines = [
            f"{game:<{26}} {owner:<{15}}"
            for game, owner in sorted(self.collection)
        ]
        lines.append("END")
        return self.file_handler.writeFile(self.path, lines)













class MergedDailyTransactions:
    def __init__(self, path, file_handler, accounts, games, collection, log):
        self.path = path
        self.file_handler = file_handler
        self.accounts = accounts
        self.games = games
        self.collection = collection
        self.log = log
        self.transactions = []

    def getAllTransactions(self):
        lines = self.file_handler.readFile(self.path)
        if self.file_handler.isBadInput(self.path, lines):
            return False
        self.transactions = lines
        return True

    @staticmethod
    def _fields(line):
        """Decode the Front End's fixed-width transaction record."""
        code = line[:2]
        if len(line) < 3 or line[2] != " ":
            raise ValueError("missing transaction-code separator")

        if code in ("00", "01", "02", "06"):
            if len(line) != 31:
                raise ValueError("Invalid user update transaction length")
            username = line[3:18].strip()
            user_type = line[19:21].strip()
            credit_amount = float(line[-9:])
            return code, username, user_type, credit_amount

        if code == "05":
            if len(line) != 44:
                raise ValueError("Invalid refund transaction length")
            buyer = line[3:18].strip()
            seller = line[19:34].strip()
            refund_amount = float(line[-9:])
            return code, buyer, seller, refund_amount

        if code == "03":
            if len(line) != 52:
                raise ValueError("Invalid sell transaction length")
            game = line[3:18].strip()
            seller = line[19:34].strip()
            price = float(line[-9:])
            return code, game, seller, price

        if code == "04":
            if len(line) != 68:
                raise ValueError("Invalid buy transaction length")
            game = line[3:29].strip()
            seller = line[30:45].strip()
            buyer = line[46:61].strip()
            price = float(line[-6:])
            return code, game, seller, buyer, price

        raise ValueError(f"Invalid transaction code \"{code}\"")

    def addUserChanges(self, transaction):
        code, username, user_type, amount = transaction
        if code == "01":
            if self.log.userExists(username, self.accounts.accounts):
                self.log.addConstraintErrors(f"Creating user that already exists ({self._current_line})")
                return
            self.accounts.addUser(username, user_type)
        elif code == "02":
            self.accounts.deleteUser(username)
            self.games.games = {
                name: item for name, item in self.games.games.items()
                if item["seller"] != username
            }

    def addAvaliableGames(self, transaction):
        code = transaction[0]
        if code == "03":
            _, game, seller, price = transaction
            self.games.addGame(game, seller, price)
        elif code == "02":
            username = transaction[1]
            self.games.games = {
                name: item for name, item in self.games.games.items()
                if item["seller"] != username
            }
        elif code == "04":
            _, game, seller, buyer, price = transaction
            self.games.deleteAvaliableGames(game)

    def addCollectionChanges(self, transaction):
        if transaction[0] == "04":
            _, game, seller, buyer, price = transaction
            self.collection.addOwnedGame(buyer, game)

    
    

    def run(self):
        if not self.getAllTransactions():
            return False

        for line in self.transactions:
            if not line.strip():
                continue
            if line[:2] == "00":
                continue

            self._current_line = line
            try:
                transaction = self._fields(line)
                code = transaction[0]

                if code == "01":
                    self.addUserChanges(transaction)
                elif code == "02":
                    self.addUserChanges(transaction)
                elif code == "06":
                    self.addUserChanges(transaction)
                elif code == "03":
                    _, game, seller, price = transaction
                    if seller not in self.accounts.accounts:
                        self.log.addConstraintErrors(
                            f"Seller isn't an existing user (transaction: {line})"
                        )
                        continue
                    self.addAvaliableGames(transaction)
                elif code == "04":
                    _, game, seller, buyer, price = transaction
                    if game not in self.games.games:
                        self.log.addConstraintErrors(
                            f"Game being bought doesn't exist (transaction: {line})"
                        )
                        continue
                    if self.collection.ownsGame(buyer, game):
                        self.log.addConstraintErrors(
                            f"Buyer buying a game that they already own (transaction: {line}"
                        )
                        continue
                    self.addAvaliableGames(transaction)
                    self.addCollectionChanges(transaction)
            except (ValueError, IndexError) as inputError:
                self.file_handler.addFatalErrors(f"Invalid transaction in \"{self.path}\": {line!r} ({inputError})")
                return False

        return True



def main():
    base_dir = Path(__file__).resolve().parent
    default_paths = (
        str(base_dir / "currentaccounts.txt"),
        str(base_dir / "availablegames.txt"),
        str(base_dir / "gamescollection.txt"),
        str(base_dir / "mergedtransactions.atf"),
    )
    accounts_path, games_path, collection_path, merged_path = sys.argv[1:5] if len(sys.argv) >= 5 else default_paths

    files = FileHandler()
    log = FailedConstraintLog()
    accounts = UserAccountsFile(accounts_path, files)
    available_games = AvailableGamesFile(games_path, files)
    game_collection = GameCollectionFile(collection_path, files)

    if not accounts.load() or not available_games.load() or not game_collection.load():
        files.displayFatalError()
        return 1

    merged = MergedDailyTransactions(
        merged_path, files, accounts, available_games, game_collection, log
    )
    
    if not merged.run():
        files.displayFatalError()
        log.displayConstraintErrors()
        return 1

    if not accounts.saveAccountInfo() or not available_games.saveGameInfo() or not game_collection.saveCollectionInfo():
        files.displayFatalError()
        return 1

    log.displayConstraintErrors()
    files.displayFatalError()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
