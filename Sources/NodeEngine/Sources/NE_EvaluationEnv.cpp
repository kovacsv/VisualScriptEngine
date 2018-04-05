#include "NE_EvaluationEnv.hpp"

namespace NE
{

EvaluationData::EvaluationData ()
{

}

EvaluationData::~EvaluationData ()
{

}

EvaluationEnv::EvaluationEnv (const std::shared_ptr<EvaluationData>& data) :
	data (data)
{

}

EvaluationEnv::~EvaluationEnv ()
{

}

EvaluationEnv EmptyEvaluationEnv (nullptr);

}
