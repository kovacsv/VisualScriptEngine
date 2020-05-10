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
	ViewerNode (const NE::String& name, const NUIE::Point& position);
	virtual ~ViewerNode ();

	virtual void						Initialize () override;
	virtual bool						IsForceCalculated () const override;

	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;
};

class MultiLineViewerNode : public NUIE::UINode
{
	DYNAMIC_SERIALIZABLE (MultiLineViewerNode);

public:
	MultiLineViewerNode ();
	MultiLineViewerNode (const NE::String& name, const NUIE::Point& position, size_t textsPerPage);
	virtual ~MultiLineViewerNode ();

	virtual void						Initialize () override;
	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NUIE::EventHandlerResult	HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;
	virtual NUIE::EventHandlerResult	HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;

	virtual bool						IsForceCalculated () const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	size_t								GetTextsPerPage () const;
	void								SetTextsPerPage (size_t newTextsPerPage);

private:
	virtual void	UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;

	size_t			GetPageCount () const;
	void			ValidateCurrentPage () const;

	size_t			textsPerPage;
	mutable size_t	currentPage;
	mutable size_t	textCount;
};

}

#endif
