@echo off
setlocal
rem Usage: run.bat [data_directory] [transaction_output_file]
rem Relative arguments are resolved from the caller's current directory.
rem Builds first so tests never silently run an outdated executable.
set "FRONTEND_DATA=%~dp0..\stored_data"
set "FRONTEND_OUTPUT=%~dp0output\daily_transaction.txt"
if not "%~1"=="" set "FRONTEND_DATA=%~f1"
if not "%~2"=="" set "FRONTEND_OUTPUT=%~f2"
for %%F in (current_user_accounts.txt available_games.txt game_collection.txt) do (
    if not exist "%FRONTEND_DATA%\%%F" (
        echo ERROR: Missing input file "%FRONTEND_DATA%\%%F".
        echo Usage: run.bat [data_directory] [transaction_output_file]
        exit /b 1
    )
)
call "%~dp0build.bat"
if errorlevel 1 exit /b 1
set "FRONTEND_EXE=%~dp0build\Debug\frontend.exe"
if not exist "%FRONTEND_EXE%" set "FRONTEND_EXE=%~dp0build\frontend.exe"
echo.
echo Data: "%FRONTEND_DATA%"
echo Transactions: "%FRONTEND_OUTPUT%"
echo Commands: login, logout, create, delete, sell, buy, refund, addcredit, listaccounts
echo After logout, press Ctrl+Z then Enter to end input and exit.
echo.

pushd "%~dp0"
"%FRONTEND_EXE%" current_user_accounts.txt available_games.txt game_collection.txt "%FRONTEND_OUTPUT%"
set "FRONTEND_RESULT=%ERRORLEVEL%"
popd

exit /b %FRONTEND_RESULT%