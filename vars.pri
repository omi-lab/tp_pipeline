TARGET = tp_pipeline
TEMPLATE = lib

DEFINES += TP_PIPELINE_LIBRARY

SOURCES += src/Globals.cpp
HEADERS += inc/tp_pipeline/Globals.h

SOURCES += src/ComplexObject.cpp
HEADERS += inc/tp_pipeline/ComplexObject.h

SOURCES += src/ComplexObjectFactory.cpp
HEADERS += inc/tp_pipeline/ComplexObjectFactory.h

SOURCES += src/ComplexObjectManager.cpp
HEADERS += inc/tp_pipeline/ComplexObjectManager.h

SOURCES += src/StepDetails.cpp
HEADERS += inc/tp_pipeline/StepDetails.h

SOURCES += src/StepContext.cpp
HEADERS += inc/tp_pipeline/StepContext.h

SOURCES += src/StepOutput.cpp
HEADERS += inc/tp_pipeline/StepOutput.h

SOURCES += src/PipelineDetails.cpp
HEADERS += inc/tp_pipeline/PipelineDetails.h

SOURCES += src/Parameter.cpp
HEADERS += inc/tp_pipeline/Parameter.h

SOURCES += src/PipelineManager.cpp
HEADERS += inc/tp_pipeline/PipelineManager.h

SOURCES += src/StepDelegate.cpp
HEADERS += inc/tp_pipeline/StepDelegate.h

SOURCES += src/StepDelegateMap.cpp
HEADERS += inc/tp_pipeline/StepDelegateMap.h

#-- Delegates --------------------------------------------------------------------------------------

SOURCES += src/step_delegates/NoneStepDelegate.cpp
HEADERS += inc/tp_pipeline/step_delegates/NoneStepDelegate.h

SOURCES += src/step_delegates/CopyStepDelegate.cpp
HEADERS += inc/tp_pipeline/step_delegates/CopyStepDelegate.h

SOURCES += src/step_delegates/InputStepDelegate.cpp
HEADERS += inc/tp_pipeline/step_delegates/InputStepDelegate.h

SOURCES += src/step_delegates/OutputStepDelegate.cpp
HEADERS += inc/tp_pipeline/step_delegates/OutputStepDelegate.h

#-- Complex objects --------------------------------------------------------------------------------

SOURCES += src/complex_objects/CollectionComplexObject.cpp
HEADERS += inc/tp_pipeline/complex_objects/CollectionComplexObject.h
