#ifndef UINODES_HPP
#define UINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"

namespace NUIE
{

class MultiLineViewerNode : public UINode
{
	DYNAMIC_SERIALIZABLE (MultiLineViewerNode);

public:
	MultiLineViewerNode ();
	MultiLineViewerNode (const std::wstring& name, const Point& position, size_t textsPerPage);
	virtual ~MultiLineViewerNode ();

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NodeParameterList& parameterList) const override;

	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	size_t						GetTextsPerPage () const;
	void						SetTextsPerPage (size_t newTextsPerPage);

private:
	virtual void	UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const override;

	size_t			GetPageCount () const;
	void			ValidateCurrentPage () const;

	size_t			textsPerPage;
	mutable size_t	currentPage;
	mutable size_t	textCount;
};

}

#endif
