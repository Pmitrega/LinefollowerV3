ECHO OFF
make clean
make all
openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program build/LFv3.elf verify reset exit"
