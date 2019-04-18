TARGET = tp_pipeline
TEMPLATE = lib

DEFINES += TP_PIPELINE_LIBRARY

SOURCES += src/Globals.cpp
HEADERS += inc/tp_pipeline/Globals.h

SOURCES += src/AbstractComplexObject.cpp
HEADERS += inc/tp_pipeline/AbstractComplexObject.h

SOURCES += src/AbstractComplexObjectFactory.cpp
HEADERS += inc/tp_pipeline/AbstractComplexObjectFactory.h

SOURCES += src/ComplexObjectManager.cpp
HEADERS += inc/tp_pipeline/ComplexObjectManager.h

SOURCES += src/StepDetails.cpp
HEADERS += inc/tp_pipeline/StepDetails.h

SOURCES += src/StepInput.cpp
HEADERS += inc/tp_pipeline/StepInput.h

SOURCES += src/PipelineDetails.cpp
HEADERS += inc/tp_pipeline/PipelineDetails.h

SOURCES += src/Parameter.cpp
HEADERS += inc/tp_pipeline/Parameter.h

SOURCES += src/PipelineManager.cpp
HEADERS += inc/tp_pipeline/PipelineManager.h

SOURCES += src/AbstractStepDelegate.cpp
HEADERS += inc/tp_pipeline/AbstractStepDelegate.h

SOURCES += src/StepDelegateMap.cpp
HEADERS += inc/tp_pipeline/StepDelegateMap.h

#-- Delegates --------------------------------------------------------------------------------------

SOURCES += src/step_delegates/NoneStepDelegate.cpp
HEADERS += inc/tp_pipeline/step_delegates/NoneStepDelegate.h

#-- Complex objects --------------------------------------------------------------------------------

SOURCES += src/complex_objects/CollectionComplexObject.cpp
HEADERS += inc/tp_pipeline/complex_objects/CollectionComplexObject.h
