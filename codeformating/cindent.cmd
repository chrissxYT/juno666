@echo off
for %%i in (*.c) do .\indent\nicec -c.\indent\nicec.cfg %%i
for %%i in (*.h) do .\indent\nicec -c.\indent\nicec.cfg %%i
for %%i in (*.cpp) do .\indent\nicec -c.\indent\nicec.cfg %%i
