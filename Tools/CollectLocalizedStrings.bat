pushd "%~dp0"
python CollectLocalizedStrings.py ..\Sources\NodeEngine ..\Sources\NodeUIEngine ..\Sources\BuiltInNodes ..\Sources\WindowsAppSupport ..\Sources\Localization\English.po
popd
