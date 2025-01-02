template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::Node_T::Node_T(Router_T* const ptrParent)
	: m_endpointData(EndpointData_T{})
	, m_realm(nullptr)
	, m_ptrParent(ptrParent)
{
	/* nothing */
}



template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::Node_T::Node_T(
	EndpointData_T data, const char* realm, Router_T* const ptrParent)
	: m_endpointData(data)
	, m_realm(realm)
	, m_ptrParent(ptrParent)
{
	/* nothing */
}



template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::Node_T::Node_T(
	const Node_T& otherNode)
	: m_endpointData(otherNode.m_endpointData)
	, m_realm(otherNode.m_realm)
	, m_ptrParent(otherNode.m_ptrParent)
{
	// nothing
};



template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::Node_T::Node_T(
	Node_T&& otherNode)
noexcept(std::is_nothrow_move_constructible_v<EndpointData_T>)
	: m_endpointData(std::move(otherNode.m_endpointData))
	, m_realm(std::exchange(otherNode.m_realm, nullptr))
	, m_ptrParent(std::exchange(otherNode.m_ptrParent, nullptr))
{
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::Node_T&
Router<EndpointData_T, URLChar_T>::Node_T::operator=(
	const Node_T& otherNode)
{
	if (this != &otherNode)
	{
		m_endpointData = otherNode.m_endpointData;
		m_realm = otherNode.m_realm;
		m_ptrParent = otherNode.m_ptrParent;
	}

	return *this;
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::Node_T&
Router<EndpointData_T, URLChar_T>::Node_T::operator=(
	Node_T&& otherNode)
noexcept(std::is_nothrow_move_assignable_v<EndpointData_T>)
{
	if (this != &otherNode)
	{
		m_endpointData = std::move(m_endpointData);
		m_realm = std::exchange(otherNode.m_realm, nullptr);
		m_ptrParent = std::exchange(otherNode.m_ptrParent, nullptr);
	}

	return *this;
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::Router_T*
Router<EndpointData_T, URLChar_T>::Node_T::Parent() const
{
	return m_ptrParent;	
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::Router_T*
Router<EndpointData_T, URLChar_T>::Node_T::Grandparent() const
{
	// data() return not valid ref
	Router_T* parent = Parent(); // for debug

	return parent->data().Parent();
};



// template <typename EndpointData_T, typename URLChar_T>
// Router<EndpointData_T, URLChar_T>::Node_T::Node_T(
// 	EndpointData_T data
// 	, std::string realm
// 	, typename Router<EndpointData_T, URLChar_T>::Router_T* const ptrParent)
	
// 	: m_endpointData(std::move(data))
// 	, m_realm(std::move(realm))
// 	, m_ptrParent(ptrParent)
// {
// 	// nothing
// };