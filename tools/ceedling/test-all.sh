#!/bin/bash -e
#
# Usage:
#   From ceedling container: 
#   sudo docker run --rm -v local/path/to/AgroIoT/:/usr/project -ti leanfrancucci/ceedling
#   cd tools/ceedling
#   ./test-all.sh
#

source_dir="../../src"
ceedling_dir="tools/ceedling"
modules="Config epoch GStatus StatQue YFrame device topic SecString"
stateMachines="Collector CommMgr"

#echo $PATH
export PATH="$PATH:/home/travis/.rvm/gems/ruby-2.4.1/bin"
#echo $PATH
#which ceedling
#exit 0

if [ ! -d $source_dir ]; then
    echo "[ERROR] This script must be invoked from "$ceedling_dir
    exit 1
fi

cleanModules()
{
    for sm in $stateMachines;
    do
        echo ""
        echo "Clean "$sm "state machine"
        echo "-------------------------"
        cd $source_dir/$sm
         if [[ ! -e project.yml || ! -e project-sm.yml || ! -e project-action.yml ]]; then
             echo "[ERROR] Ceedling project not found"
             exit 1
         else
            ceedling clean
            ceedling clobber
         fi
    done
    currdir=$PWD
    for module in $modules;
    do
        echo ""
        echo "Clean "$module "module"
        echo "----------------------"
        cd $source_dir/$module
        if [ ! -e "project.yml" ]; then
            echo "[ERROR] Ceedling project not found"
            exit 1
        else
            ceedling clean
            ceedling clobber
        fi
    done
}

testModuleExceptions()
{
    echo ""
    echo "Run all test of ffile module"
    echo "----------------------------"
    cd $source_dir/ffile
    if [ ! -e "project.yml" ]; then
        echo "[ERROR] Ceedling project not found"
        exit 1
    else
        if [ $clobber == 0 ]; then
            ceedling clean gcov:ffile
            ceedling clean options:project-ffdir gcov:ffdir
            ceedling clean options:project-complete-eeprom gcov:ffile-complete
            ceedling clean options:project-complete-dataflash gcov:ffile-complete
        else
            ceedling clean clobber gcov:ffile
            ceedling clean clobber options:project-ffdir gcov:ffdir
            ceedling clean clobber options:project-complete-eeprom gcov:ffile-complete
            ceedling clean clobber options:project-complete-dataflash gcov:ffile-complete
        fi
    fi

    echo ""
    echo "Run all test of Backup module"
    echo "-----------------------------"
    cd $source_dir/Backup
    if [ ! -e "project.yml" ]; then
        echo "[ERROR] Ceedling project not found"
        exit 1
    else
        if [ $clobber == 0 ]; then
            ceedling clean options:project-complete gcov:complete
            ceedling clean gcov:Backup
        else
            ceedling clean clobber options:project-complete gcov:complete
            ceedling clean clobber gcov:Backup
        fi
    fi
}

coverModuleExceptions()
{
    echo ""
    echo "Generating code coverage report for ffile"
    echo "-----------------------------------------"
    cd $currdir
    cd $source_dir/ffile
    lcov -e ../../$ceedling_dir/gcov/coverage-total.info "$(pwd)/src/ffile.c" -o ../../$ceedling_dir/gcov/ffile.info
    lcov -e ../../$ceedling_dir/gcov/coverage-total.info "$(pwd)/src/devflash.c" -o ../../$ceedling_dir/gcov/devflash.info
    lcov -e ../../$ceedling_dir/gcov/coverage-total.info "$(pwd)/src/rfile.c" -o ../../$ceedling_dir/gcov/rfile.info
    add+=(-a devflash.info)
    add+=(-a ffile.info)
    add+=(-a rfile.info)

    echo ""
    echo "Generating code coverage report for Backup"
    echo "------------------------------------------"
    cd $currdir
    cd $source_dir/Backup
    lcov -e ../../$ceedling_dir/gcov/coverage-total.info "$(pwd)/src/Backup.c" -o ../../$ceedling_dir/gcov/Backup.info
    add+=(-a Backup.info)
}

case "$1" in
    clean)
        clobber=1
        ;;
    clobber)
        cleanModules
        exit 0
        ;;
    *)
        clobber=0
        ;;
esac

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
             ceedling clean gcov:all
         else
             ceedling clean clobber gcov:all
         fi
     fi
done

for sm in $stateMachines;
do
    echo ""
    echo "Run all test of "$sm "state machine"
    echo "-----------------------------------"
    cd $source_dir/$sm
     if [[ ! -e "project.yml" || ! -e "project-sm.yml" || ! -e "project-action.yml" ]]; then
         echo "[ERROR] Ceedling project not found"
         exit 1
     else
         if [ $clobber == 0 ]; then
             ceedling clean options:project-sm gcov:$sm
             ceedling clean options:project-action gcov:$sm"Act"
         else
             ceedling clean clobber options:project-sm gcov:$sm
             ceedling clean clobber options:project-action gcov:$sm"Act"
         fi
     fi
done

testModuleExceptions

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

coverModuleExceptions

echo ""
echo "Generating complete code coverage report"
echo "----------------------------------------"
cd ../../$ceedling_dir/gcov/
lcov "${add[@]}" -o coverage.info
genhtml coverage.info -o .

exit 0

