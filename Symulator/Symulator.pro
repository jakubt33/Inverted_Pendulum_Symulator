#-------------------------------------------------
#
# Project created by QtCreator 2017-06-15T11:44:48
#
#-------------------------------------------------

QT  += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Symulator
TEMPLATE = app

INCLUDEPATH += $$PWD/FANN/include/
INCLUDEPATH += $$PWD/Fuzzy/

CONFIG += precompile_header

SOURCES += main.cpp\
        mainwindow.cpp \
    pendulum.cpp \
    Embed/Motors/Motors.c \
    Embed/PID/PID.c \
    Embed/PID/PID_Usr.c \
    qcustomplot.cpp \
    charts.cpp \
    FANN/fann.c \
    FANN/fann_cascade.c \
    FANN/fann_error.c \
    FANN/fann_io.c \
    FANN/fann_train.c \
    FANN/fann_train_data.c \
    neuralnetwork.cpp \
    Fuzzy/src/activation/First.cpp \
    Fuzzy/src/activation/General.cpp \
    Fuzzy/src/activation/Highest.cpp \
    Fuzzy/src/activation/Last.cpp \
    Fuzzy/src/activation/Lowest.cpp \
    Fuzzy/src/activation/Proportional.cpp \
    Fuzzy/src/activation/Threshold.cpp \
    Fuzzy/src/defuzzifier/Bisector.cpp \
    Fuzzy/src/defuzzifier/Centroid.cpp \
    Fuzzy/src/defuzzifier/IntegralDefuzzifier.cpp \
    Fuzzy/src/defuzzifier/LargestOfMaximum.cpp \
    Fuzzy/src/defuzzifier/MeanOfMaximum.cpp \
    Fuzzy/src/defuzzifier/SmallestOfMaximum.cpp \
    Fuzzy/src/defuzzifier/WeightedAverage.cpp \
    Fuzzy/src/defuzzifier/WeightedAverageCustom.cpp \
    Fuzzy/src/defuzzifier/WeightedDefuzzifier.cpp \
    Fuzzy/src/defuzzifier/WeightedSum.cpp \
    Fuzzy/src/defuzzifier/WeightedSumCustom.cpp \
    Fuzzy/src/factory/ActivationFactory.cpp \
    Fuzzy/src/factory/DefuzzifierFactory.cpp \
    Fuzzy/src/factory/FactoryManager.cpp \
    Fuzzy/src/factory/FunctionFactory.cpp \
    Fuzzy/src/factory/HedgeFactory.cpp \
    Fuzzy/src/factory/SNormFactory.cpp \
    Fuzzy/src/factory/TermFactory.cpp \
    Fuzzy/src/factory/TNormFactory.cpp \
    Fuzzy/src/hedge/Any.cpp \
    Fuzzy/src/hedge/Extremely.cpp \
    Fuzzy/src/hedge/HedgeFunction.cpp \
    Fuzzy/src/hedge/Not.cpp \
    Fuzzy/src/hedge/Seldom.cpp \
    Fuzzy/src/hedge/Somewhat.cpp \
    Fuzzy/src/hedge/Very.cpp \
    Fuzzy/src/imex/CppExporter.cpp \
    Fuzzy/src/imex/Exporter.cpp \
    Fuzzy/src/imex/FclExporter.cpp \
    Fuzzy/src/imex/FclImporter.cpp \
    Fuzzy/src/imex/FisExporter.cpp \
    Fuzzy/src/imex/FisImporter.cpp \
    Fuzzy/src/imex/FldExporter.cpp \
    Fuzzy/src/imex/FllExporter.cpp \
    Fuzzy/src/imex/FllImporter.cpp \
    Fuzzy/src/imex/Importer.cpp \
    Fuzzy/src/imex/JavaExporter.cpp \
    Fuzzy/src/imex/RScriptExporter.cpp \
    Fuzzy/src/norm/s/AlgebraicSum.cpp \
    Fuzzy/src/norm/s/BoundedSum.cpp \
    Fuzzy/src/norm/s/DrasticSum.cpp \
    Fuzzy/src/norm/s/EinsteinSum.cpp \
    Fuzzy/src/norm/s/HamacherSum.cpp \
    Fuzzy/src/norm/s/Maximum.cpp \
    Fuzzy/src/norm/s/NilpotentMaximum.cpp \
    Fuzzy/src/norm/s/NormalizedSum.cpp \
    Fuzzy/src/norm/s/SNormFunction.cpp \
    Fuzzy/src/norm/s/UnboundedSum.cpp \
    Fuzzy/src/norm/t/AlgebraicProduct.cpp \
    Fuzzy/src/norm/t/BoundedDifference.cpp \
    Fuzzy/src/norm/t/DrasticProduct.cpp \
    Fuzzy/src/norm/t/EinsteinProduct.cpp \
    Fuzzy/src/norm/t/HamacherProduct.cpp \
    Fuzzy/src/norm/t/Minimum.cpp \
    Fuzzy/src/norm/t/NilpotentMinimum.cpp \
    Fuzzy/src/norm/t/TNormFunction.cpp \
    Fuzzy/src/rule/Antecedent.cpp \
    Fuzzy/src/rule/Consequent.cpp \
    Fuzzy/src/rule/Expression.cpp \
    Fuzzy/src/rule/Rule.cpp \
    Fuzzy/src/rule/RuleBlock.cpp \
    Fuzzy/src/term/Activated.cpp \
    Fuzzy/src/term/Aggregated.cpp \
    Fuzzy/src/term/Bell.cpp \
    Fuzzy/src/term/Binary.cpp \
    Fuzzy/src/term/Concave.cpp \
    Fuzzy/src/term/Constant.cpp \
    Fuzzy/src/term/Cosine.cpp \
    Fuzzy/src/term/Discrete.cpp \
    Fuzzy/src/term/Function.cpp \
    Fuzzy/src/term/Gaussian.cpp \
    Fuzzy/src/term/GaussianProduct.cpp \
    Fuzzy/src/term/Linear.cpp \
    Fuzzy/src/term/PiShape.cpp \
    Fuzzy/src/term/Ramp.cpp \
    Fuzzy/src/term/Rectangle.cpp \
    Fuzzy/src/term/Sigmoid.cpp \
    Fuzzy/src/term/SigmoidDifference.cpp \
    Fuzzy/src/term/SigmoidProduct.cpp \
    Fuzzy/src/term/Spike.cpp \
    Fuzzy/src/term/SShape.cpp \
    Fuzzy/src/term/Term.cpp \
    Fuzzy/src/term/Trapezoid.cpp \
    Fuzzy/src/term/Triangle.cpp \
    Fuzzy/src/term/ZShape.cpp \
    Fuzzy/src/variable/InputVariable.cpp \
    Fuzzy/src/variable/OutputVariable.cpp \
    Fuzzy/src/variable/Variable.cpp \
    Fuzzy/src/Benchmark.cpp \
    Fuzzy/src/Complexity.cpp \
    Fuzzy/src/Console.cpp \
    Fuzzy/src/Engine.cpp \
    Fuzzy/src/Exception.cpp \
    Fuzzy/src/fuzzylite.cpp \
    Fuzzy/src/fl_main.cpp \
    fuzzycontroller.cpp

HEADERS  += mainwindow.h \
    pendulum.h \
    Embed/Motors/Motors.h \
    Embed/PID/PID.h \
    Embed/PID/PID_Usr.h \
    commonDefines.h \
    qcustomplot.h \
    charts.h \
    FANN/include/compat_time.h \
    FANN/include/config.h \
    #FANN/include/doublefann.h \
    FANN/include/fann.h \
    FANN/include/fann_activation.h \
    FANN/include/fann_cascade.h \
    FANN/include/fann_data.h \
    FANN/include/fann_error.h \
    FANN/include/fann_internal.h \
    FANN/include/fann_io.h \
    FANN/include/fann_train.h \
    #FANN/include/floatfann.h \
    Fuzzy/pre_struct.h \
    Fuzzy/user_param.h \
    Fuzzy/user_rules.h \
    Fuzzy/fl/activation/Activation.h \
    Fuzzy/fl/activation/First.h \
    Fuzzy/fl/activation/General.h \
    Fuzzy/fl/activation/Highest.h \
    Fuzzy/fl/activation/Last.h \
    Fuzzy/fl/activation/Lowest.h \
    Fuzzy/fl/activation/Proportional.h \
    Fuzzy/fl/activation/Threshold.h \
    Fuzzy/fl/defuzzifier/Bisector.h \
    Fuzzy/fl/defuzzifier/Centroid.h \
    Fuzzy/fl/defuzzifier/Defuzzifier.h \
    Fuzzy/fl/defuzzifier/IntegralDefuzzifier.h \
    Fuzzy/fl/defuzzifier/LargestOfMaximum.h \
    Fuzzy/fl/defuzzifier/MeanOfMaximum.h \
    Fuzzy/fl/defuzzifier/SmallestOfMaximum.h \
    Fuzzy/fl/defuzzifier/WeightedAverage.h \
    Fuzzy/fl/defuzzifier/WeightedAverageCustom.h \
    Fuzzy/fl/defuzzifier/WeightedDefuzzifier.h \
    Fuzzy/fl/defuzzifier/WeightedSum.h \
    Fuzzy/fl/defuzzifier/WeightedSumCustom.h \
    Fuzzy/fl/factory/ActivationFactory.h \
    Fuzzy/fl/factory/CloningFactory.h \
    Fuzzy/fl/factory/ConstructionFactory.h \
    Fuzzy/fl/factory/DefuzzifierFactory.h \
    Fuzzy/fl/factory/FactoryManager.h \
    Fuzzy/fl/factory/FunctionFactory.h \
    Fuzzy/fl/factory/HedgeFactory.h \
    Fuzzy/fl/factory/SNormFactory.h \
    Fuzzy/fl/factory/TermFactory.h \
    Fuzzy/fl/factory/TNormFactory.h \
    Fuzzy/fl/hedge/Any.h \
    Fuzzy/fl/hedge/Extremely.h \
    Fuzzy/fl/hedge/Hedge.h \
    Fuzzy/fl/hedge/HedgeFunction.h \
    Fuzzy/fl/hedge/Not.h \
    Fuzzy/fl/hedge/Seldom.h \
    Fuzzy/fl/hedge/Somewhat.h \
    Fuzzy/fl/hedge/Very.h \
    Fuzzy/fl/imex/CppExporter.h \
    Fuzzy/fl/imex/Exporter.h \
    Fuzzy/fl/imex/FclExporter.h \
    Fuzzy/fl/imex/FclImporter.h \
    Fuzzy/fl/imex/FisExporter.h \
    Fuzzy/fl/imex/FisImporter.h \
    Fuzzy/fl/imex/FldExporter.h \
    Fuzzy/fl/imex/FllExporter.h \
    Fuzzy/fl/imex/FllImporter.h \
    Fuzzy/fl/imex/Importer.h \
    Fuzzy/fl/imex/JavaExporter.h \
    Fuzzy/fl/imex/RScriptExporter.h \
    Fuzzy/fl/norm/s/AlgebraicSum.h \
    Fuzzy/fl/norm/s/BoundedSum.h \
    Fuzzy/fl/norm/s/DrasticSum.h \
    Fuzzy/fl/norm/s/EinsteinSum.h \
    Fuzzy/fl/norm/s/HamacherSum.h \
    Fuzzy/fl/norm/s/Maximum.h \
    Fuzzy/fl/norm/s/NilpotentMaximum.h \
    Fuzzy/fl/norm/s/NormalizedSum.h \
    Fuzzy/fl/norm/s/SNormFunction.h \
    Fuzzy/fl/norm/s/UnboundedSum.h \
    Fuzzy/fl/norm/t/AlgebraicProduct.h \
    Fuzzy/fl/norm/t/BoundedDifference.h \
    Fuzzy/fl/norm/t/DrasticProduct.h \
    Fuzzy/fl/norm/t/EinsteinProduct.h \
    Fuzzy/fl/norm/t/HamacherProduct.h \
    Fuzzy/fl/norm/t/Minimum.h \
    Fuzzy/fl/norm/t/NilpotentMinimum.h \
    Fuzzy/fl/norm/t/TNormFunction.h \
    Fuzzy/fl/norm/Norm.h \
    Fuzzy/fl/norm/SNorm.h \
    Fuzzy/fl/norm/TNorm.h \
    Fuzzy/fl/rule/Antecedent.h \
    Fuzzy/fl/rule/Consequent.h \
    Fuzzy/fl/rule/Expression.h \
    Fuzzy/fl/rule/Rule.h \
    Fuzzy/fl/rule/RuleBlock.h \
    Fuzzy/fl/term/Activated.h \
    Fuzzy/fl/term/Aggregated.h \
    Fuzzy/fl/term/Bell.h \
    Fuzzy/fl/term/Binary.h \
    Fuzzy/fl/term/Concave.h \
    Fuzzy/fl/term/Constant.h \
    Fuzzy/fl/term/Cosine.h \
    Fuzzy/fl/term/Discrete.h \
    Fuzzy/fl/term/Function.h \
    Fuzzy/fl/term/Gaussian.h \
    Fuzzy/fl/term/GaussianProduct.h \
    Fuzzy/fl/term/Linear.h \
    Fuzzy/fl/term/PiShape.h \
    Fuzzy/fl/term/Ramp.h \
    Fuzzy/fl/term/Rectangle.h \
    Fuzzy/fl/term/Sigmoid.h \
    Fuzzy/fl/term/SigmoidDifference.h \
    Fuzzy/fl/term/SigmoidProduct.h \
    Fuzzy/fl/term/Spike.h \
    Fuzzy/fl/term/SShape.h \
    Fuzzy/fl/term/Term.h \
    Fuzzy/fl/term/Trapezoid.h \
    Fuzzy/fl/term/Triangle.h \
    Fuzzy/fl/term/ZShape.h \
    Fuzzy/fl/variable/InputVariable.h \
    Fuzzy/fl/variable/OutputVariable.h \
    Fuzzy/fl/variable/Variable.h \
    Fuzzy/fl/Benchmark.h \
    Fuzzy/fl/Complexity.h \
    Fuzzy/fl/Console.h \
    Fuzzy/fl/Engine.h \
    Fuzzy/fl/Exception.h \
    Fuzzy/fl/fuzzylite.h \
    Fuzzy/fl/Headers.h \
    Fuzzy/fl/Operation.h \
    fuzzycontroller.h \
    neuralnetwork.h

FORMS    += mainwindow.ui \
    charts.ui

DISTFILES += \
    Fuzzy/readme.md \
    FANN/include/CMakeLists.txt \
    FANN/CMakeLists.txt
