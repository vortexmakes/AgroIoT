#!/bin/bash -e
#

#source_dir="../../src"
source_dir="../../tracking"
ceedling_dir="tools/ceedling"
modules="ffile"
stateMachines=""
coverage=0
clobber=0
run="test"

#echo $PATH
export PATH="$PATH:/home/travis/.rvm/gems/ruby-2.4.1/bin"
#echo $PATH
#which ceedling
#exit 0

runTest() {
    currdir=$PWD
    for module in $modules;
    do
        echo ""
        echo "Run all test of "$module "module"
        echo "--------------------------------"
        cd $source_dir/$module
        if [ ! -e "project.yml" ]; then
            echo "[ERROR] Ceedling project not found"
            exit 1
        else
            if [ $clobber == 0 ]; then
                ceedling clean $run:all
            else
                ceedling clean clobber $run:all
            fi
        fi
    done
}

runTestStateMachine() {
    for sm in $stateMachines;
    do
        echo ""
        echo "Run all test of "$sm "state machine"
        echo "-----------------------------------"
        cd $source_dir/$sm
         if [ [ ! -e "project.yml" ] && 
              [ ! -e "project-sm.yml" ] && 
              [ ! -e "project-action.yml" ] ]; then
             echo "[ERROR] Ceedling project not found"
             exit 1
         else
             if [ $clobber == 0 ]; then
                 ceedling clean options:project-sm $run:$sm
                 ceedling clean options:project-action $run:$sm"Act"
             else
                 ceedling clean clobber options:project-sm $run:$sm
                 ceedling clean clobber options:project-action $run:$sm"Act"
             fi
         fi
    done
}

runCoverage() {
    echo ""
    echo "Generating code coverage report for modules"
    echo "-------------------------------------------"
    cd ..
    lcov -c -d . -o ../$ceedling_dir/gcov/coverage-total.info
    add=()
    for module in $modules;
    do
        cd $currdir
        cd $source_dir/$module
        lcov -e ../../$ceedling_dir/gcov/coverage-total.info "$(pwd)/src/$module*.c" -o ../../$ceedling_dir/gcov/$module.info
        add+=(-a $module".info")
#   cd $source_dir/$module/test/
#   submodules=(`find . -name "test_rkh*.c" -type f | sort -r | head -8 | sed 's/\.\\/test_//' | sed 's/\.c//'`)
#   cd ..
#   find build/gcov/out/ ! -name rkh$module*.gc* -type f | xargs sudo rm -f
    done
}    

runCoverageStateMachine() {
    echo ""
    echo "Generating code coverage report for state machines"
    echo "--------------------------------------------------"
    for sm in $stateMachines;
    do
        cd $currdir
        cd $source_dir/$sm
        #lcov -e ../../$ceedling_dir/gcov/coverage-total.info "$(pwd)/src/$sm.c" -o ../../$ceedling_dir/gcov/$sm.info
        #add+=(-a $sm".info")
        lcov -e ../../$ceedling_dir/gcov/coverage-total.info "$(pwd)/src/$sm"Act".c" -o ../../$ceedling_dir/gcov/$sm"Act".info
        add+=(-a $sm"Act"".info")
    done
}

coverageReport() {
    echo ""
    echo "Generating complete code coverage report"
    echo "----------------------------------------"
    cd ../../$ceedling_dir/gcov/
    lcov "${add[@]}" -o coverage.info
    genhtml coverage.info -o .
}

if [ ! -d $source_dir ]; then
    echo "[ERROR] This script must be invoked from "$ceedling_dir
    exit 1
fi

checkCoverage() {
    if [ $coverage == 1 ]; then
        run="gcov"
    fi
}

usage() {
    name=$(echo $0 | sed 's/\.\///')
    echo "Usage: $name [-c] [-v]"
    echo "-c: clean and clobber before running test cases"
    echo "-v: run test case with code coverage"
}

exit_abnormal() {
    usage
    exit 1
}

while getopts "cvh" opt;
do
    case ${opt} in
        c)
            clobber=1
            ;;
        v)
            coverage=1
            ;;
        h)
            usage
            exit 0
            ;;
        \?)
            exit_abnormal
            ;;
    esac
done

checkCoverage
runTest
runTestStateMachine
if [ $coverage == 1 ]; then
    runCoverage
    runCoverageStateMachine
    coverageReport
fi

exit 0

