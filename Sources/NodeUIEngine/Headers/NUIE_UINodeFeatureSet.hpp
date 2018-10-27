#ifndef UINODEFEATURESET_HPP
#define UINODEFEATURESET_HPP

#include "NE_Stream.hpp"
#include "NE_Serializable.hpp"

#include <memory>
#include <vector>
#include <unordered_map>

namespace NUIE
{

class NodeCommandRegistrator;
class NodeParameterList;

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

class UINodeFeature;
typedef std::shared_ptr<UINodeFeature> UINodeFeaturePtr;
typedef std::shared_ptr<const UINodeFeature> UINodeFeatureConstPtr;

class UINodeFeature : public NE::DynamicSerializable
{
	SERIALIZABLE;

public:
	UINodeFeature ();
	UINodeFeature (const FeatureId& featureId);
	virtual ~UINodeFeature ();

	const FeatureId&			GetId () const;

	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const = 0;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const = 0;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream);
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	template <class Type>
	static Type* Cast (UINodeFeature* feature);

	template <class Type>
	static std::shared_ptr<Type> Cast (const UINodeFeaturePtr& node);

private:
	FeatureId	featureId;
};

template <class Type>
Type* UINodeFeature::Cast (UINodeFeature* feature)
{
	return dynamic_cast<Type*> (feature);
}

template <class Type>
std::shared_ptr<Type> UINodeFeature::Cast (const UINodeFeaturePtr& feature)
{
	return std::dynamic_pointer_cast<Type> (feature);
}

}

namespace std
{
	template <>
	struct hash<NUIE::FeatureId>
	{
		size_t operator() (const NUIE::FeatureId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

namespace NUIE
{

class UINodeFeatureSet
{
	SERIALIZABLE;

public:
	UINodeFeatureSet ();
	~UINodeFeatureSet ();

	void						AddFeature (const FeatureId& featureId, const UINodeFeaturePtr& feature);
	bool						HasFeature (const FeatureId& featureId) const;
	const UINodeFeaturePtr&		GetFeature (const FeatureId& featureId) const;

	void						RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const;
	void						RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	NE::Stream::Status			Read (NE::InputStream& inputStream);
	NE::Stream::Status			Write (NE::OutputStream& outputStream) const;

private:
	std::vector<UINodeFeaturePtr>			features;
	std::unordered_map<FeatureId, size_t>	idToIndex;
};

}

#endif
