#include "ResultImage.hpp"

ResultImage::ResultImage () :
	modified (false)
{

}

ResultImage::~ResultImage ()
{

}

bool ResultImage::IsModified () const
{
	return modified;
}

void ResultImage::Draw (NUIE::DrawingContext& context) const
{
	drawingImage.Draw (context);
}

void ResultImage::AddItem (const NUIE::DrawingItemConstPtr& item)
{
	drawingImage.AddItem (item);
	Invalidate ();
}

void ResultImage::RemoveItem (const NUIE::DrawingItemConstPtr& item)
{
	drawingImage.RemoveItem (item);
	Invalidate ();
}

void ResultImage::Invalidate ()
{
	modified = true;
}

void ResultImage::Validate ()
{
	modified = false;
}

void ResultImage::Clear ()
{
	drawingImage.Clear ();
	Invalidate ();
}

ResultImageEvaluationData::ResultImageEvaluationData (const std::shared_ptr<ResultImage>& resultImage) :
	resultImage (resultImage)
{

}

ResultImageEvaluationData::~ResultImageEvaluationData ()
{

}

std::shared_ptr<ResultImage>& ResultImageEvaluationData::GetResultImage ()
{
	return resultImage;
}

void ResultImageEvaluationData::AddItem (const NUIE::DrawingItemConstPtr& item)
{
	resultImage->AddItem (item);
}

void ResultImageEvaluationData::RemoveItem (const NUIE::DrawingItemConstPtr& item)
{
	resultImage->RemoveItem (item);
}
