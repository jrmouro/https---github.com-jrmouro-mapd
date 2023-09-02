#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Circle.o \
	${OBJECTDIR}/Rectangle.o \
	${OBJECTDIR}/Render.o \
	${OBJECTDIR}/Shape.o \
	${OBJECTDIR}/Text.o \
	${OBJECTDIR}/_agent.o \
	${OBJECTDIR}/_agent_charging.o \
	${OBJECTDIR}/_agent_charging_CL.o \
	${OBJECTDIR}/_agent_dead.o \
	${OBJECTDIR}/_agent_goingToCharging.o \
	${OBJECTDIR}/_agent_goingToCharging_CL.o \
	${OBJECTDIR}/_agent_goingToDelivery.o \
	${OBJECTDIR}/_agent_goingToDelivery_CL.o \
	${OBJECTDIR}/_agent_goingToPickup.o \
	${OBJECTDIR}/_agent_goingToPickup_CL.o \
	${OBJECTDIR}/_agent_goingToRest.o \
	${OBJECTDIR}/_agent_goingToRest_CL.o \
	${OBJECTDIR}/_agent_parked.o \
	${OBJECTDIR}/_agent_parken_CL.o \
	${OBJECTDIR}/_agent_state.o \
	${OBJECTDIR}/_map.o \
	${OBJECTDIR}/_token.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1 \
	${TESTDIR}/TestFiles/f2

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/newsimpletest.o \
	${TESTDIR}/tests/newsimpletest1.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs sfml-all`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapd

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapd: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mapd ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Circle.o: Circle.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Circle.o Circle.cpp

${OBJECTDIR}/Rectangle.o: Rectangle.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Rectangle.o Rectangle.cpp

${OBJECTDIR}/Render.o: Render.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Render.o Render.cpp

${OBJECTDIR}/Shape.o: Shape.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Shape.o Shape.cpp

${OBJECTDIR}/Text.o: Text.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Text.o Text.cpp

${OBJECTDIR}/_agent.o: _agent.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent.o _agent.cpp

${OBJECTDIR}/_agent_charging.o: _agent_charging.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_charging.o _agent_charging.cpp

${OBJECTDIR}/_agent_charging_CL.o: _agent_charging_CL.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_charging_CL.o _agent_charging_CL.cpp

${OBJECTDIR}/_agent_dead.o: _agent_dead.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_dead.o _agent_dead.cpp

${OBJECTDIR}/_agent_goingToCharging.o: _agent_goingToCharging.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToCharging.o _agent_goingToCharging.cpp

${OBJECTDIR}/_agent_goingToCharging_CL.o: _agent_goingToCharging_CL.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToCharging_CL.o _agent_goingToCharging_CL.cpp

${OBJECTDIR}/_agent_goingToDelivery.o: _agent_goingToDelivery.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToDelivery.o _agent_goingToDelivery.cpp

${OBJECTDIR}/_agent_goingToDelivery_CL.o: _agent_goingToDelivery_CL.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToDelivery_CL.o _agent_goingToDelivery_CL.cpp

${OBJECTDIR}/_agent_goingToPickup.o: _agent_goingToPickup.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToPickup.o _agent_goingToPickup.cpp

${OBJECTDIR}/_agent_goingToPickup_CL.o: _agent_goingToPickup_CL.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToPickup_CL.o _agent_goingToPickup_CL.cpp

${OBJECTDIR}/_agent_goingToRest.o: _agent_goingToRest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToRest.o _agent_goingToRest.cpp

${OBJECTDIR}/_agent_goingToRest_CL.o: _agent_goingToRest_CL.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToRest_CL.o _agent_goingToRest_CL.cpp

${OBJECTDIR}/_agent_parked.o: _agent_parked.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_parked.o _agent_parked.cpp

${OBJECTDIR}/_agent_parken_CL.o: _agent_parken_CL.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_parken_CL.o _agent_parken_CL.cpp

${OBJECTDIR}/_agent_state.o: _agent_state.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_state.o _agent_state.cpp

${OBJECTDIR}/_map.o: _map.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_map.o _map.cpp

${OBJECTDIR}/_token.o: _token.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_token.o _token.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/newsimpletest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/newsimpletest1.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/newsimpletest.o: tests/newsimpletest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newsimpletest.o tests/newsimpletest.cpp


${TESTDIR}/tests/newsimpletest1.o: tests/newsimpletest1.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. `pkg-config --cflags sfml-all` -std=c++14  -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/newsimpletest1.o tests/newsimpletest1.cpp


${OBJECTDIR}/Circle_nomain.o: ${OBJECTDIR}/Circle.o Circle.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Circle.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Circle_nomain.o Circle.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Circle.o ${OBJECTDIR}/Circle_nomain.o;\
	fi

${OBJECTDIR}/Rectangle_nomain.o: ${OBJECTDIR}/Rectangle.o Rectangle.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Rectangle.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Rectangle_nomain.o Rectangle.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Rectangle.o ${OBJECTDIR}/Rectangle_nomain.o;\
	fi

${OBJECTDIR}/Render_nomain.o: ${OBJECTDIR}/Render.o Render.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Render.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Render_nomain.o Render.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Render.o ${OBJECTDIR}/Render_nomain.o;\
	fi

${OBJECTDIR}/Shape_nomain.o: ${OBJECTDIR}/Shape.o Shape.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Shape.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Shape_nomain.o Shape.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Shape.o ${OBJECTDIR}/Shape_nomain.o;\
	fi

${OBJECTDIR}/Text_nomain.o: ${OBJECTDIR}/Text.o Text.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Text.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Text_nomain.o Text.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Text.o ${OBJECTDIR}/Text_nomain.o;\
	fi

${OBJECTDIR}/_agent_nomain.o: ${OBJECTDIR}/_agent.o _agent.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_nomain.o _agent.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent.o ${OBJECTDIR}/_agent_nomain.o;\
	fi

${OBJECTDIR}/_agent_charging_nomain.o: ${OBJECTDIR}/_agent_charging.o _agent_charging.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_charging.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_charging_nomain.o _agent_charging.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_charging.o ${OBJECTDIR}/_agent_charging_nomain.o;\
	fi

${OBJECTDIR}/_agent_charging_CL_nomain.o: ${OBJECTDIR}/_agent_charging_CL.o _agent_charging_CL.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_charging_CL.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_charging_CL_nomain.o _agent_charging_CL.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_charging_CL.o ${OBJECTDIR}/_agent_charging_CL_nomain.o;\
	fi

${OBJECTDIR}/_agent_dead_nomain.o: ${OBJECTDIR}/_agent_dead.o _agent_dead.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_dead.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_dead_nomain.o _agent_dead.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_dead.o ${OBJECTDIR}/_agent_dead_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToCharging_nomain.o: ${OBJECTDIR}/_agent_goingToCharging.o _agent_goingToCharging.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToCharging.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToCharging_nomain.o _agent_goingToCharging.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToCharging.o ${OBJECTDIR}/_agent_goingToCharging_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToCharging_CL_nomain.o: ${OBJECTDIR}/_agent_goingToCharging_CL.o _agent_goingToCharging_CL.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToCharging_CL.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToCharging_CL_nomain.o _agent_goingToCharging_CL.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToCharging_CL.o ${OBJECTDIR}/_agent_goingToCharging_CL_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToDelivery_nomain.o: ${OBJECTDIR}/_agent_goingToDelivery.o _agent_goingToDelivery.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToDelivery.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToDelivery_nomain.o _agent_goingToDelivery.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToDelivery.o ${OBJECTDIR}/_agent_goingToDelivery_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToDelivery_CL_nomain.o: ${OBJECTDIR}/_agent_goingToDelivery_CL.o _agent_goingToDelivery_CL.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToDelivery_CL.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToDelivery_CL_nomain.o _agent_goingToDelivery_CL.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToDelivery_CL.o ${OBJECTDIR}/_agent_goingToDelivery_CL_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToPickup_nomain.o: ${OBJECTDIR}/_agent_goingToPickup.o _agent_goingToPickup.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToPickup.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToPickup_nomain.o _agent_goingToPickup.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToPickup.o ${OBJECTDIR}/_agent_goingToPickup_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToPickup_CL_nomain.o: ${OBJECTDIR}/_agent_goingToPickup_CL.o _agent_goingToPickup_CL.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToPickup_CL.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToPickup_CL_nomain.o _agent_goingToPickup_CL.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToPickup_CL.o ${OBJECTDIR}/_agent_goingToPickup_CL_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToRest_nomain.o: ${OBJECTDIR}/_agent_goingToRest.o _agent_goingToRest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToRest.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToRest_nomain.o _agent_goingToRest.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToRest.o ${OBJECTDIR}/_agent_goingToRest_nomain.o;\
	fi

${OBJECTDIR}/_agent_goingToRest_CL_nomain.o: ${OBJECTDIR}/_agent_goingToRest_CL.o _agent_goingToRest_CL.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_goingToRest_CL.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_goingToRest_CL_nomain.o _agent_goingToRest_CL.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_goingToRest_CL.o ${OBJECTDIR}/_agent_goingToRest_CL_nomain.o;\
	fi

${OBJECTDIR}/_agent_parked_nomain.o: ${OBJECTDIR}/_agent_parked.o _agent_parked.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_parked.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_parked_nomain.o _agent_parked.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_parked.o ${OBJECTDIR}/_agent_parked_nomain.o;\
	fi

${OBJECTDIR}/_agent_parken_CL_nomain.o: ${OBJECTDIR}/_agent_parken_CL.o _agent_parken_CL.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_parken_CL.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_parken_CL_nomain.o _agent_parken_CL.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_parken_CL.o ${OBJECTDIR}/_agent_parken_CL_nomain.o;\
	fi

${OBJECTDIR}/_agent_state_nomain.o: ${OBJECTDIR}/_agent_state.o _agent_state.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_agent_state.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_agent_state_nomain.o _agent_state.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_agent_state.o ${OBJECTDIR}/_agent_state_nomain.o;\
	fi

${OBJECTDIR}/_map_nomain.o: ${OBJECTDIR}/_map.o _map.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_map.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_map_nomain.o _map.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_map.o ${OBJECTDIR}/_map_nomain.o;\
	fi

${OBJECTDIR}/_token_nomain.o: ${OBJECTDIR}/_token.o _token.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_token.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_token_nomain.o _token.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_token.o ${OBJECTDIR}/_token_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags sfml-all` -std=c++14  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
