#CC = mips-openwrt-linux-gcc 
#CXX = mips-openwrt-linux-g++ 

CPPFLAGS = -I..
CFLAGS = -std=gnu99 -Wall -Wno-unused-parameter -Wno-unused-function -O3
CXXFLAGS = -Wall -O3




#LDFLAGS =  -L /home/ale/mips/OpenWrt-Toolchain-ar71xx-for-mips_r2-gcc-4.6-linaro_uClibc-0.9.33.2/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/ -ldl -lpthread -lm -lstdc++

#LDFLAGS+=jpeg-9c/.libs/libjpeg.a -lm -lpthread -lstdc++
LDFLAGS+=-ljpeg -lm -lpthread -lstdc++
#LDFLAGS+=-L/home/ale/mips/cam_localization_for_mips-master/apriltags_mips/example/jpeg-9c/
SRCS=utils
INCS=-I. -I$(SRCS) -Iexample/jpeg-9c

# OBJS =  $(PROG).o \
#         $(SRCS)/YUYVtoJPEG.o



TARGETS := apriltag_demo 

# TARGETS := apriltag_demo #opencv_demo

.PHONY: all
# all: apriltag_demo opencv_demo
all: apriltag_demo

foo.o: foo.cpp
	g++ -c foo.cpp -O3 -I../../eigen-323c052e1731

apriltag_demo: apriltag_demo.o ../libapriltag.a v4l2.o $(SRCS)/YUYVtoJPEG.o $(SRCS)/RAWtoFILE.o foo.o
	@echo "   [$@]"
	@$(CC) -o $@ $^ $(LDFLAGS)

opencv_demo: opencv_demo.o ../libapriltag.a
	@echo "   [$@]"
	@$(CXX) -o $@ $^ $(LDFLAGS) `pkg-config --libs opencv`

%.o: %.c
	@echo "   $@"
	@$(CC) -o $@ -c $< $(CFLAGS) $(INCS) $(CPPFLAGS)

%.o: %.cc
	@echo "   $@"
	@$(CXX) -o $@ -c $< $(CXXFLAGS) $(CPPFLAGS)

.PHONY: clean
clean:
	@rm -rf *.o utils/*.o $(TARGETS)
