#include "NUIE_ParameterDialog.hpp"

#include "NE_SingleValues.hpp"
#include "NUIE_NodeParameters.hpp"

namespace NUIE
{

static const int DialogPadding = 5;
static const int StaticWidth = 60;
static const int ControlWidth = 150;
static const int ControlHeight = 12;
static const int ButtonWidth = 50;
static const int ButtonHeight = 15;

class ChangedParameter
{
public:
	ChangedParameter (size_t index, const NE::ValuePtr& value) :
		index (index),
		value (value)
	{

	}

	size_t GetIndex () const
	{
		return index;
	}

	const NE::ValuePtr& GetValue () const
	{
		return value;
	}

private:
	size_t			index;
	NE::ValuePtr	value;
};

ParameterDialogBase::ParameterDialogBase (const std::wstring& dialogTitle, const ParameterInterfacePtr& paramInterface) :
	dialogTitle (dialogTitle),
	paramInterface (paramInterface),
	changedParamIndices ()
{

}

ParameterDialogBase::~ParameterDialogBase ()
{

}

bool ParameterDialogBase::Show (int x, int y)
{
	size_t paramCount = paramInterface->GetParameterCount ();
	int dialogInnerWidth = StaticWidth + ControlWidth + DialogPadding;
	int dialogWidth = dialogInnerWidth + 2 * DialogPadding;
	int dialogHeight = (int) paramCount * ControlHeight + ((int) paramCount + 3) * DialogPadding + ButtonHeight;
	IntRect dialogRect (x, y, dialogWidth, dialogHeight);
	SetDialogRect (dialogRect);

	int currentY = DialogPadding;
	for (size_t paramIndex = 0; paramIndex < paramCount; ++paramIndex) {
		NUIE::ParameterType type = paramInterface->GetParameterType (paramIndex);

		IntRect nameStaticRect (DialogPadding, currentY, StaticWidth, ControlHeight);
		AddParamNameStatic (paramIndex, paramInterface->GetParameterName (paramIndex).c_str (), nameStaticRect);

		NE::ValueConstPtr value = paramInterface->GetParameterValue (paramIndex);
		if (type == NUIE::ParameterType::Boolean) {
			if (DBGVERIFY (NE::Value::IsType<NE::BooleanValue> (value))) {
				int selectedChoice = NE::BooleanValue::Get (value) ? 0 : 1;
				std::vector<std::wstring> choices = { NE::LocalizeString (L"true"), NE::LocalizeString (L"false") };
				IntRect controlRect (StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight);
				AddParamComboBox (paramIndex, selectedChoice, choices, controlRect);
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				IntRect controlRect (StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight);
				AddParamEditText (paramIndex, controlText, controlRect);
			}
		} else if (type == NUIE::ParameterType::Float) {
			if (DBGVERIFY (NE::Value::IsType<NE::FloatValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				IntRect controlRect (StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight);
				AddParamEditText (paramIndex, controlText, controlRect);
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				IntRect controlRect (StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight);
				AddParamEditText (paramIndex, controlText, controlRect);
			}
		} else if (type == NUIE::ParameterType::String) {
			if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				IntRect controlRect (StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight);
				AddParamEditText (paramIndex, controlText, controlRect);
			}
		} else if (type == NUIE::ParameterType::Enumeration) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				int selectedChoice = NE::IntValue::Get (value);
				std::vector<std::wstring> choices = paramInterface->GetParameterValueChoices (paramIndex);
				IntRect controlRect (StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight);
				AddParamComboBox (paramIndex, selectedChoice, choices, controlRect);
			}
		} else {
			DBGBREAK ();
		}

		currentY += ControlHeight + DialogPadding;
	}

	AddHorizontalSeparator (DialogPadding, currentY, dialogInnerWidth);
	IntRect cancelButtonRect (dialogInnerWidth - 2 * ButtonWidth, currentY + DialogPadding, ButtonWidth, ButtonHeight);
	IntRect okButtonRect (dialogInnerWidth - ButtonWidth + DialogPadding, currentY + DialogPadding, ButtonWidth, ButtonHeight);
	AddCancelButton (NE::LocalizeString (L"Cancel"), cancelButtonRect);
	AddOkButton (NE::LocalizeString (L"OK"), okButtonRect);

	return ShowDialog ();
}

void ParameterDialogBase::SetParameterChanged (size_t paramIndex)
{
	changedParamIndices.insert (paramIndex);
}

bool ParameterDialogBase::ApplyParameterChanges ()
{
	std::vector<ChangedParameter> changedParamValues;
	bool isAllParameterValid = true;
	for (size_t paramIndex = 0; paramIndex < paramInterface->GetParameterCount (); ++paramIndex) {
		if (changedParamIndices.find (paramIndex) == changedParamIndices.end ()) {
			continue;
		}

		NUIE::ParameterType type = paramInterface->GetParameterType (paramIndex);

		NE::ValuePtr value;
		if (type == NUIE::ParameterType::Boolean) {
			int selectedItem = GetComboboxSelectedItem (paramIndex);
			value = NE::ValuePtr (new NE::BooleanValue (selectedItem == 0 ? true : false));
		} else if (type == NUIE::ParameterType::Integer || type == NUIE::ParameterType::Float || type == NUIE::ParameterType::Double || type == NUIE::ParameterType::String) {
			std::wstring itemText = GetEditTextValue (paramIndex);
			value = NUIE::StringToParameterValue (std::wstring (itemText), type);
		} else if (type == NUIE::ParameterType::Enumeration) {
			int selectedItem = GetComboboxSelectedItem (paramIndex);
			value = NE::ValuePtr (new NE::IntValue (selectedItem));
		} else {
			DBGBREAK ();
			continue;
		}

		if (paramInterface->IsValidParameterValue (paramIndex, value)) {
			ChangedParameter param (paramIndex, value);
			changedParamValues.push_back (param);
		} else {
			NE::ValueConstPtr oldValue = paramInterface->GetParameterValue (paramIndex);
			std::wstring oldValueString = NUIE::ParameterValueToString (oldValue, type);
			SetEditTextValue (paramIndex, oldValueString);
			isAllParameterValid = false;
		}
	}

	if (!isAllParameterValid) {
		return false;
	}
	
	for (const ChangedParameter& param : changedParamValues) {
		paramInterface->SetParameterValue (param.GetIndex (), param.GetValue ());
	}
	return true;
}

}
