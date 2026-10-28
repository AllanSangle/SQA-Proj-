@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem sets directory locations
set "ROOT=%~dp0"
set "FRONTEND=%ROOT%Frontend"
set "TEST_CASES=%ROOT%test_cases"
set "RESULTS=%ROOT%test_runs"

@REM  creates the report file for the test summary, and adds a title and time of when the test run occured
set "REPORT=%RESULTS%\test_summary.txt"
if not exist "%RESULTS%" mkdir "%RESULTS%"
> "%REPORT%" echo Game Distribution System: Test Summary
>> "%REPORT%" echo Time Of Test Run: %DATE% %TIME%
>> "%REPORT%" echo.

echo Game Distribution System Tester
echo.

rem Builds the program
call "%FRONTEND%\build.bat"

rem Says if the program failed to build
if errorlevel 1 (
    echo ERROR: Failed to build the program
    exit /b 1
)

rem Sets the frontend.exe file to run the program
set "EXE=%FRONTEND%\build\Debug\frontend.exe"

rem creates the directory to save the test results if it hasn't been created
if not exist "%RESULTS%\" mkdir "%RESULTS%"

pushd "%FRONTEND%"

rem keeps track of the number of fails for each type
set /a TEST_COUNT=0
set /a FAILED_RUN_COUNT=0
set /a FAILED_TERMINAL_COUNT=0
set /a FAILED_TRANSACTION_COUNT=0
set /a MISSING_FILE_COUNT=0

rem navigates through test case categories, to the test case, then the success or failure folders
for /d %%C in ("%TEST_CASES%\*") do (
	for /d %%T in ("%%~fC\*") do (
		for /d %%V in ("%%~fT\*") do (

			rem Sets the input file to use for the tests, checking for both Input and input for potential inconsistent naming
			set "INPUT_FILE="
			if exist "%%~fV\Input.txt" set "INPUT_FILE=%%~fV\Input.txt"
			if not defined INPUT_FILE if exist "%%~fV\input.txt" set "INPUT_FILE=%%~fV\input.txt"
		
			if defined INPUT_FILE (
                rem saves the results, including the actual output of the program and the daily transaction file
                set "RESULT_DIR=%RESULTS%\%%~nxC\%%~nxT\%%~nxV"
                set "TERMINAL_OUTPUT=!RESULT_DIR!\actual_output.txt"
                set "TRANSACTION_OUTPUT=!RESULT_DIR!\daily_transaction.atf"

                rem sets the output file for the expected output of a test, checking for both Output and output
                set "EXPECTED_TERMINAL=%%~fV\Output.txt"
                if not exist "!EXPECTED_TERMINAL!" set "EXPECTED_TERMINAL=%%~fV\output.txt"


                rem sets the expected daily transaction output from the test run
                set "EXPECTED_TRANSACTION=%%~fV\daily_transaction.txt"

                rem make the directory for saving results if it doesnt exist
                if not exist "!RESULT_DIR!\" mkdir "!RESULT_DIR!"

                rem deletes the outputs from previous test runs
                if exist "!TERMINAL_OUTPUT!" del /q "!TERMINAL_OUTPUT!"
                if exist "!TRANSACTION_OUTPUT!" del /q "!TRANSACTION_OUTPUT!"

                set /a TEST_COUNT+=1

                echo.
                echo [RUNNING TEST:] %%~nxC/%%~nxT/%%~nxV
                echo.

                rem Runs the program as a single command line with the inputs and output folder with the three input filenames and transaction output path.
                "!EXE!" current_user_accounts.txt available_games.txt game_collection.txt "!TRANSACTION_OUTPUT!" < "!INPUT_FILE!" > "!TERMINAL_OUTPUT!"
                
                rem tests exist on errors
                set "TEST_EXIT=!ERRORLEVEL!"

                rem Add the output file even if no session was started, to show that there was a failure if nothing is displayed or run.
                if not exist "!TRANSACTION_OUTPUT!" type nul > "!TRANSACTION_OUTPUT!"

                if not "!TEST_EXIT!"=="0" (
                    echo [RUN ERROR] Program exited with code !TEST_EXIT!.
                    set /a FAILED_RUN_COUNT+=1
                )

                rem Compare the actual terminal output from the test with the expected terminal output, update error counts, and log results
                if not exist "!EXPECTED_TERMINAL!" (
                    echo [EXPECTED OUTPUT FILE MISSING] "!EXPECTED_TERMINAL!"
                    set /a MISSING_FILE_COUNT+=1
                    set /a FAILED_TERMINAL_COUNT+=1
                ) else (
                    fc /b "!EXPECTED_TERMINAL!" "!TERMINAL_OUTPUT!" >nul 2>&1
                    if errorlevel 1 (
                        echo [FAILED TERMINAL MATCH] %%~nxT/%%~nxV
			            >> "%REPORT%" echo [FAILED TERMINAL MATCH] %%~nxC/%%~nxT/%%~nxV
                        echo Expected: "!EXPECTED_TERMINAL!"
                        echo Actual: "!TERMINAL_OUTPUT!"
                        set /a FAILED_TERMINAL_COUNT+=1
                    ) else (
                        echo [PASSED TERMINAL MATCH]
			            >> "%REPORT%" echo [PASSED TERMINAL MATCH] %%~nxC/%%~nxT/%%~nxV
                    )
                )

                rem compare the actual daily transaction file from the test with the expected daily transaction file, update error counts, and log results
                if not exist "!EXPECTED_TRANSACTION!" (
                    echo [DAILY TRANSACTION FILE MISSING] "!EXPECTED_TRANSACTION!"
                    set /a MISSING_FILE_COUNT+=1
                    set /a FAILED_TRANSACTION_COUNT+=1
                ) else (
                    fc /b "!EXPECTED_TRANSACTION!" "!TRANSACTION_OUTPUT!" >nul 2>&1
                    if errorlevel 1 (
                        echo [FAILED TRANSACTION MATCH] %%~nxT/%%~nxV
			            >> "%REPORT%" echo [FAILED TRANSACTION MATCH] %%~nxC/%%~nxT/%%~nxV
                        echo Expected: "!EXPECTED_TRANSACTION!"
                        echo Actual: "!TRANSACTION_OUTPUT!"
                        set /a FAILED_TRANSACTION_COUNT+=1
                    ) else (
                        echo [PASSED TRANSACTION MATCH]
			            >> "%REPORT%" echo [PASSED TRANSACTION MATCH] %%~nxC/%%~nxT/%%~nxV
                    )
                )
            )
        )
    )
)

popd

echo.
echo Test Summary
echo.

rem gives a summary about the test results and where to locate the results file
echo Tests Ran: !TEST_COUNT!
echo Program Run Errors: !FAILED_RUN_COUNT!
echo Terminal Output Inconsistencies: !FAILED_TERMINAL_COUNT!
echo Transaction File Inconsistencies: !FAILED_TRANSACTION_COUNT!
echo Missing Files: !MISSING_FILE_COUNT!
echo Actual results: "%RESULTS%"

rem Saves summary to the report file
>> "%REPORT%" echo.
>> "%REPORT%" echo Tests Ran: !TEST_COUNT!
>> "%REPORT%" echo Program Run Errors: !FAILED_RUN_COUNT!
>> "%REPORT%" echo Terminal Output Inconsistencies: !FAILED_TERMINAL_COUNT!
>> "%REPORT%" echo Transaction File Inconsistencies: !FAILED_TRANSACTION_COUNT!
>> "%REPORT%" echo Missing Files: !MISSING_FILE_COUNT!


if "!TEST_COUNT!"=="0" (
    echo ERROR: Failed to locate test input files
    >> "%REPORT%" echo ERROR: Failed to locate test input files
    exit /b 1
)

if not "!FAILED_RUN_COUNT!"=="0" exit /b 1
if not "!FAILED_TERMINAL_COUNT!"=="0" exit /b 1
if not "!FAILED_TRANSACTION_COUNT!"=="0" exit /b 1
if not "!MISSING_FILE_COUNT!"=="0" exit /b 1

echo Program Passed All Tests

exit /b 0