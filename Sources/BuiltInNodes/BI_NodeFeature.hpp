#ifndef BI_NODEFEATURE_HPP
#define BI_NODEFEATURE_HPP

#include "NE_Stream.hpp"
#include "NE_Serializable.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeCommonParameters.hpp"

#include <memory>

namespace BI
{

class FeatureId
{
	SERIALIZABLE;

public:
	FeatureId ();
	FeatureId (const std::string& id);
	~FeatureId ();

	size_t				GenerateHashValue () const;

	bool				operator< (const FeatureId& rhs) const;
	bool				operator> (const FeatureId& rhs) const;
	bool				operator== (const FeatureId& rhs) const;
	bool				operator!= (const FeatureId& rhs) const;

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	std::string id;
};

class NodeFeature;
using NodeFeaturePtr = std::shared_ptr<NodeFeature>;
using NodeFeatureConstPtr = std::shared_ptr<const NodeFeature>;

class NodeFeature : public NE::DynamicSerializable
{
	SERIALIZABLE;

public:
	NodeFeature ();
	NodeFeature (const FeatureId& featureId);
	virtual ~NodeFeature ();

	const FeatureId&			GetId () const;

	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const = 0;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const = 0;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	template <class Type>
	static Type* Cast (NodeFeature* feature);

	template <class Type>
	static std::shared_ptr<Type> Cast (const NodeFeaturePtr& node);

private:
	FeatureId	featureId;
};

template <class Type>
Type* NodeFeature::Cast (NodeFeature* feature)
{
	return dynamic_cast<Type*> (feature);
}

template <class Type>
std::shared_ptr<Type> NodeFeature::Cast (const NodeFeaturePtr& feature)
{
	return std::dynamic_pointer_cast<Type> (feature);
}

}

namespace std
{
	template <>
	struct hash<BI::FeatureId>
	{
		size_t operator() (const BI::FeatureId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

#endif
