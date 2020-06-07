#ifndef BI_UINODES_HPP
#define BI_UINODES_HPP

#include "NE_SingleValues.hpp"
#include "NUIE_UINode.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"

namespace BI
{

class ViewerNode : public BasicUINode
{
	DYNAMIC_SERIALIZABLE (ViewerNode);

public:
	class Layout : public HeaderWithSlotsAndTextLayout
	{
	public:
		Layout ();

		virtual std::wstring	GetText (const BasicUINode& uiNode, const NE::StringConverter& stringConverter) const override;
	};

	ViewerNode ();
	ViewerNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~ViewerNode ();

	virtual void						Initialize () override;
	virtual bool						IsForceCalculated () const override;

	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;
};

class MultiLineViewerNode : public BasicUINode
{
	DYNAMIC_SERIALIZABLE (MultiLineViewerNode);

public:
	class Layout : public HeaderWithSlotsAndMultilineTextLayout
	{
	public:
		Layout (const std::string& leftButtonId,
				const std::wstring& leftButtonText,
				const std::string& rightButtonId,
				const std::wstring& rightButtonText);

		virtual void	GetTextInfo (	const BasicUINode& uiNode,
										const NE::StringConverter& stringConverter,
										std::vector<std::wstring>& texts,
										size_t& textsPerPage) const override;
	};

	MultiLineViewerNode ();
	MultiLineViewerNode (const NE::LocString& name, const NUIE::Point& position, size_t textsPerPage);
	virtual ~MultiLineViewerNode ();

	virtual void						Initialize () override;
	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual bool						IsForceCalculated () const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	size_t								GetTextsPerPage () const;
	void								SetTextsPerPage (size_t newTextsPerPage);

private:
	size_t								textsPerPage;
};

}

#endif
