pushd "%~dp0"
python CollectLocalizedStrings.py ..\Sources\NodeEngine ..\Sources\NodeUIEngine ..\Sources\BuiltInNodes ..\Sources\WindowsAppSupport ..\Build\DevKit\localization\english.po
popd

