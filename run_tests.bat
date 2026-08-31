@echo off
setlocal enabledelayedexpansion

set LOGFILE=test_results.txt
set TESTS=texture_test hitbox_test renderer_test font_test sprite_test animation_test input_test character_test hud_test round_timer_test camera_test audio_manager_test config_test

echo Test run started: %DATE% %TIME% > %LOGFILE%
echo ============================================== >> %LOGFILE%

echo Building all tests...
mingw32-make tests >> %LOGFILE% 2>&1

echo. >> %LOGFILE%
echo ============================================== >> %LOGFILE%
echo BUILD DONE - RUNNING TESTS >> %LOGFILE%
echo ============================================== >> %LOGFILE%

for %%T in (%TESTS%) do (
    echo. >> %LOGFILE%
    echo ---------------------------------------------- >> %LOGFILE%
    echo Running %%T.exe >> %LOGFILE%
    echo ---------------------------------------------- >> %LOGFILE%

    if exist %%T.exe (
        %%T.exe >> %LOGFILE% 2>&1
        echo Exit code: !ERRORLEVEL! >> %LOGFILE%
    ) else (
        echo [SKIPPED] %%T.exe not found - build failed >> %LOGFILE%
    )
)

echo. >> %LOGFILE%
echo ============================================== >> %LOGFILE%
echo Test run finished: %DATE% %TIME% >> %LOGFILE%

echo Done. Results in %LOGFILE%