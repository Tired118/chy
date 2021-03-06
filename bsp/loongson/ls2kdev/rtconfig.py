import os
# CPU options
ARCH='mips'
CPU ='gs264'

if os.getenv('RTT_ROOT'):
    RTT_ROOT = os.getenv('RTT_ROOT')
else:
    RTT_ROOT = '../../..'

# toolchains options
CROSS_TOOL  = 'gcc'

if os.getenv('RTT_CC'):
	CROSS_TOOL = os.getenv('RTT_CC')

if  CROSS_TOOL == 'gcc':
	PLATFORM    = 'gcc'
	EXEC_PATH   = r"C:\mgc\embedded\codebench\bin"
#	EXEC_PATH   = r'D:\mgc\embedded\codebench\bin'
else:
    print('================ERROR===========================')
    print('Not support %s yet!' % CROSS_TOOL)
    print('=================================================')
    exit(0)

if os.getenv('RTT_EXEC_PATH'):
	EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD       = 'debug'

PREFIX = 'mips-sde-elf-'
CC = PREFIX + 'gcc'
CXX = PREFIX + 'g++'
AS = PREFIX + 'gcc'
AR = PREFIX + 'ar'
LINK = PREFIX + 'gcc'
TARGET_EXT = 'elf'
SIZE = PREFIX + 'size'
OBJDUMP = PREFIX + 'objdump'
OBJCPY = PREFIX + 'objcopy'
READELF = PREFIX + 'readelf'

DEVICE = ' -march=mips64r2 -mabi=64 -msoft-float -EL -mno-gpopt'
CFLAGS = DEVICE + '  -G0 -mno-abicalls -fno-pic -fno-builtin -fno-exceptions -ffunction-sections -fomit-frame-pointer'
AFLAGS = ' -c' + DEVICE + '  -fno-pic -fno-builtin -mno-abicalls -x assembler-with-cpp'
LFLAGS = DEVICE + ' -nostartfiles -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,Reset_Handler -T ls2k_ram.lds'
CXXFLAGS = CFLAGS

CPATH = ''
LPATH = ''

if BUILD == 'debug':
    CFLAGS += ' -O0 -gdwarf-2'
    AFLAGS += ' -gdwarf-2'
else:
    CFLAGS += ' -O2'

DUMP_ACTION = OBJDUMP + ' -D -S $TARGET > rtt.asm\n'
READELF_ACTION = READELF + ' -a $TARGET > rtt.map\n'
POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' + SIZE + ' $TARGET \n'
