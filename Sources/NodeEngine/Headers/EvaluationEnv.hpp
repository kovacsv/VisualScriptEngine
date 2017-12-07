#ifndef EVALUATIONENVIRONMENT_HPP
#define EVALUATIONENVIRONMENT_HPP

#include <memory>

namespace NE
{

class EvaluationData
{
public:
	EvaluationData ();
	virtual ~EvaluationData ();
};

class EvaluationEnv
{
public:
	EvaluationEnv (const std::shared_ptr<EvaluationData>& data);
	~EvaluationEnv ();

	template <typename T>
	bool IsDataType () const;

	template <typename T>
	std::shared_ptr<T> GetData ();

private:
	std::shared_ptr<EvaluationData> data;
};

template <typename T>
bool EvaluationEnv::IsDataType () const
{
	return std::dynamic_pointer_cast<T> (data) != nullptr;
}

template <typename T>
std::shared_ptr<T> EvaluationEnv::GetData ()
{
	return std::dynamic_pointer_cast<T> (data);
}

extern EvaluationEnv EmptyEvaluationEnv;

}

#endif
