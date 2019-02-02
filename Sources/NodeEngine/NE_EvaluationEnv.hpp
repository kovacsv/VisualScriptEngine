#ifndef NE_EVALUATIONENVIRONMENT_HPP
#define NE_EVALUATIONENVIRONMENT_HPP

#include <memory>

namespace NE
{

class EvaluationData
{
public:
	EvaluationData ();
	virtual ~EvaluationData ();
};

typedef std::shared_ptr<EvaluationData> EvaluationDataPtr;
typedef std::shared_ptr<const EvaluationData> EvaluationDataConstPtr;

class EvaluationEnv
{
public:
	EvaluationEnv (const EvaluationDataPtr& data);
	~EvaluationEnv ();

	template <typename T>
	bool IsDataType () const;

	template <typename T>
	std::shared_ptr<T> GetData ();

private:
	EvaluationDataPtr data;
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
