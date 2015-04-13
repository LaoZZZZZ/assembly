CC	= mpic++
CFLAGS	= -std=c++11 -fPIC -w -I$(BOOST_DIR)
PROG	= run


# command

RM	=rm
CP	=cp
CD	=cd
MV	=mv

	
SOURCES	= ./src/mtpi_send_reads.cpp ./src/messagesender.cpp ./src/mtpi_build.cpp ./src/nodemap_util.cpp ./src/main.cpp ./src/init_bcast_group.cpp ./src/messagesenderreceiver.cpp ./src/mtpi_listen.cpp ./src/messagereceiver.cpp ./src/master_thread.cpp ./src/globalparameter.cpp ./src/parameter.cpp ./src/time_stamp.cpp ./src/messageprocessor.cpp ./src/operation.cpp ./src/get_ip_address.cpp ./src/mtpi_init.cpp ./src/noderole_int2str.cpp ./src/tasktag_int2str.cpp ./src/processor.cpp ./src/mtpi_send.cpp ./src/rawmessage.cpp ./src/mtpi_encode_nodeinfo.cpp ./src/stopwatch.cpp ./src/mtpi_barrier.cpp ./src/nodestatus_int2str.cpp ./src/netcomm.cpp ./src/mtpi_bcast.cpp ./src/engine_thread.cpp ./src/mtpi_send_genome.cpp ./src/mtpi_finalize.cpp ./src/messagegenerator.cpp ./src/mtpithread.cpp ./src/mtpi_write_info.cpp ./src/assemblyparameter.cpp ./src/threadwrapper.cpp ./src/generatetable.cpp ./src/mtpi_decode_nodeinfo.cpp ./src/msgsize2readable.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run
all: $(SOURCES) $(EXECUTABLE)
clean:
	$(RM) $(OBJECTS)
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
.cpp.o:
	$(CC)  -c $(CFLAGS) $< -o $@

