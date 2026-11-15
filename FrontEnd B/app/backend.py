# Backend Prototype

import sys
from pathlib import Path

class FileHander:
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

    def addFatalErrors(self, fatalError):
        self.fatal_errors.append(f"ERROR: (FATAL) {fatalError}")

    def displayFatalError(self):
        for error in self.fatal_errors:
            print(error)
