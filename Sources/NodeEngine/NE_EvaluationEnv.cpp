#include "NE_EvaluationEnv.hpp"

namespace NE
{

EvaluationData::EvaluationData ()
{

}

EvaluationData::~EvaluationData ()
{

}

EvaluationEnv::EvaluationEnv (const EvaluationDataPtr& data) :
	data (data)
{

}

EvaluationEnv::~EvaluationEnv ()
{

}

EvaluationEnv EmptyEvaluationEnv (nullptr);

}
