#######################################################################
#             FILE  NAME  :    Makefile
#             FILE  DESC  :    Makefile的模板文件
#             AUTHOR      :    zhifuli
#             #             DATE        :    20100610
#######################################################################

LIB_PATH =  . /usr/lib64/mysql/

LIBS = 

INC_PATH = . 

#######################################################################

.SUFFIXES: .o .cc
	        
CXX        = g++
CFLAGS     = -g  -Wall -DLINUX64
RANLIB     = ranlib

.cc.o: 
	$(CXX) $(CFLAGS)  -c $^ $(addprefix -I,$(INC_PATH)) 
.c.o: 
	gcc $(CFLAGS)  -c $^ $(addprefix -I,$(INC_PATH))
			        
#######################################################################################         

#OBJECT = b_login_login2.o b_login2_login.o b_login_twice.o b_login2_twice.o
OBJECT = b_send_login3_with_valid_ver.o

#######################################################################################

#OUTPUT = b_login_login2 b_login2_login b_login_twice b_login2_twice
OUTPUT = b_send_login3_with_valid_ver

########################################################################

all:$(OUTPUT)

$(OUTPUT) : $(OBJECT)
	$(CXX) $(CFLAGS) -o $@   $^  $(addprefix -L,$(LIB_PATH))  $(addprefix -l,$(LIBS))  -lpthread -lmysqlclient

clean: 
	rm  *.o   $(OUTPUT)


