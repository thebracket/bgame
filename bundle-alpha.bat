@echo Building Pre-Alpha Release
@md c:\nfbuild > nul 2> nul
@SET bundleDir=c:\nfbuild\NoxFutura-%DATE:~-4%-%DATE:~4,2%-%DATE:~7,2%
@md %bundleDir%  > nul 2> nul
@md %bundleDir%\game_assets > nul 2> nul
@md %bundleDir%\game_assets\cursors > nul 2> nul
@md %bundleDir%\game_assets\shaders45 > nul 2> nul
@md %bundleDir%\game_assets\shaders45\ascii > nul 2> nul
@md %bundleDir%\game_assets\shaders45\compute > nul 2> nul
@md %bundleDir%\game_assets\shaders45\worldgen > nul 2> nul
@md %bundleDir%\game_assets\rex > nul 2> nul
@md %bundleDir%\game_assets\terrain > nul 2> nul
@md %bundleDir%\game_assets\vox > nul 2> nul
@md %bundleDir%\game_assets\worldgen > nul 2> nul
@md %bundleDir%\world_defs > nul 2> nul
@SET exeDirectory=NoxFuturaVS\x64\Publish\
@copy %exeDirectory%*.dll %bundleDir% > nul 2> nul
@copy %exeDirectory%NoxFuturaVS.exe %bundleDir%\NoxFutura.exe > nul 2> nul
@copy game_assets\*.* %bundleDir%\game_assets > nul 2> nul
@copy game_assets\shaders45\*.* %bundleDir%\game_assets\shaders45 > nul 2> nul
@copy game_assets\shaders45\ascii\*.* %bundleDir%\game_assets\shaders45\ascii > nul 2> nul
@copy game_assets\shaders45\compute\*.* %bundleDir%\game_assets\shaders45\compute > nul 2> nul
@copy game_assets\shaders45\worldgen\*.* %bundleDir%\game_assets\shaders45\worldgen > nul 2> nul
@copy game_assets\cursors\*.* %bundleDir%\game_assets\cursors > nul 2> nul
@copy game_assets\rex\*.* %bundleDir%\game_assets\rex > nul 2> nul
@copy game_assets\terrain\*.* %bundleDir%\game_assets\terrain > nul 2> nul
@copy game_assets\vox\*.* %bundleDir%\game_assets\vox > nul 2> nul
@copy game_assets\worldgen\*.* %bundleDir%\game_assets\worldgen > nul 2> nul
@copy world_defs\*.* %bundleDir%\world_defs > nul 2> nul
@echo Copied Bundle
@SET butler=%APPDATA%\itch\bin\butler.exe
@%butler% upgrade
@SET uploadCommand=%butler% push %bundleDir% thebracket/nox-futura:windows-pre-alpha
@%uploadCommand%
