#ifndef UINODES_HPP
#define UINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"

namespace NUIE
{

class MultiLineViewerUINode : public UINode
{
	DYNAMIC_SERIALIZABLE (MultiLineViewerUINode);

public:
	MultiLineViewerUINode ();
	MultiLineViewerUINode (const std::wstring& name, const Point& position, size_t textsPerPage);
	virtual ~MultiLineViewerUINode ();

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;

	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual void	UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const override;
	virtual void	CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;

	size_t			GetPageCount () const;
	void			ValidateCurrentPage () const;

	size_t			textsPerPage;
	mutable size_t	currentPage;
	mutable size_t	textCount;
};

}

#endif
