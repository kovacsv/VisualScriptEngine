#ifndef UINODES_HPP
#define UINODES_HPP

#include "NE_SingleValues.hpp"
#include "NUIE_UINode.hpp"

namespace BI
{

class MultiLineViewerNode : public NUIE::UINode
{
	DYNAMIC_SERIALIZABLE (MultiLineViewerNode);

public:
	MultiLineViewerNode ();
	MultiLineViewerNode (const std::wstring& name, const NUIE::Point& position, size_t textsPerPage);
	virtual ~MultiLineViewerNode ();

	virtual void						RegisterSlots () override;
	virtual NE::ValuePtr				Calculate (NE::EvaluationEnv& env) const override;
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NUIE::EventHandlerResult	HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position) override;

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
