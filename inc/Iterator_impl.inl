#include <memory> // addressof

template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::LNRIterator::LNRIterator(
	Router<EndpointData_T, URLChar_T>::NativeIter_T nativeIter)
	: m_nativeIter(nativeIter)
{
	// nothing
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::NativeIter_T
Router<EndpointData_T, URLChar_T>::LNRIterator::GetIterFromThis(
	Router<EndpointData_T, URLChar_T>::Router_T* node) const
{
	Router_T* parentOfNode = node->data().Parent();
	NativeIter_T nativeIterSameDeep = parentOfNode->begin();

	while (std::addressof(nativeIterSameDeep->second) != node)
	{
		nativeIterSameDeep++;
	}

	return nativeIterSameDeep;
};



template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::LNRIterator::LNRIterator(
	Router<EndpointData_T, URLChar_T>::Router_T& node)
	: m_nativeIter(GetIterFromThis(std::addressof(node)))
{
	// nothing
};



template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::LNRIterator::LNRIterator(
	Router<EndpointData_T, URLChar_T>::Router_T* node)
	: m_nativeIter(GetIterFromThis(node))
{
	// nothing
};



template <typename EndpointData_T, typename URLChar_T>
bool
Router<EndpointData_T, URLChar_T>::LNRIterator::HasChild() const noexcept
{
	return GetRouter().begin() == GetRouter().end();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::Router_T&
Router<EndpointData_T, URLChar_T>::LNRIterator::GetRouter() const noexcept
{
	return (*m_nativeIter).second;
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::LNRIterator&
Router<EndpointData_T, URLChar_T>::LNRIterator::operator++()
{
	if (HasChild())
	{
		m_nativeIter = GetRouter().begin();
	} else
	{
		Router_T* parent = Data().Parent(); // parent always exists
		if (m_nativeIter != parent->end())
		{
			m_nativeIter++;
		} else
		{
			LNRIterator parentIter = LNRIterator{Data().Parent()};
			*this = ++parentIter;
		}

	}
	
	
	return *this;
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::LNRIterator
Router<EndpointData_T, URLChar_T>::LNRIterator::operator++(int)
{
	NativeIter_T retIter = m_nativeIter;
	
	if (HasChild())
	{
		m_nativeIter = GetRouter().begin();
	} else
	{
		Router_T* parent = Data().Parent(); // parent always exists
		if (m_nativeIter != parent->end())
		{
			m_nativeIter++;
		} else
		{
			LNRIterator parentIter = LNRIterator{Data().Parent()};
			*this = ++parentIter;
		}

	}

	return  retIter;
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::LNRIterator::reference
Router<EndpointData_T, URLChar_T>::LNRIterator::Data() const
{
	return m_nativeIter->second.data();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::LNRIterator::reference
Router<EndpointData_T, URLChar_T>::LNRIterator::operator*() const
{
	return this->Data();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::LNRIterator::pointer
Router<EndpointData_T, URLChar_T>::LNRIterator::operator->() const
{
	return std::addressof(Data());
};



template <typename EndpointData_T, typename URLChar_T>
bool
Router<EndpointData_T, URLChar_T>::LNRIterator::operator==(LNRIterator otherIter) const
{
	return m_nativeIter == otherIter.m_nativeIter;
};



template <typename EndpointData_T, typename URLChar_T>
bool
Router<EndpointData_T, URLChar_T>::LNRIterator::operator!=(LNRIterator otherIter) const
{
	return not this->operator==(otherIter);
};



// template <typename EndpointData_T, typename URLChar_T>
// Router<EndpointData_T, URLChar_T>::LNRIterator::LNRIterator(
// 	Router<EndpointData_T, URLChar_T>::LNRIterator::reference node)
// 	: m_nativeIter(GetIterFromThis(std::addressof(node)))
// {
//	// nothing
// };



// =================================================
// =========== struct const_LNRIterator ============
// =================================================

template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::const_LNRIterator::const_LNRIterator(
	Router<EndpointData_T, URLChar_T>::ConstNativeIter_T constNativeIter)
	: m_constNativeIter(constNativeIter)
{
	// nothing
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::ConstNativeIter_T
Router<EndpointData_T, URLChar_T>::const_LNRIterator::GetIterFromThis(
	const Router<EndpointData_T, URLChar_T>::Router_T* node) const
{
	const Router_T* parentOfNode = node->data().Parent();

	return std::find_if(parentOfNode.begin(), parentOfNode.end(), 
		[node](const LNRIterator& child){
			return std::addressof(child.second) == node;
		});
};



template <typename EndpointData_T, typename URLChar_T>
const typename Router<EndpointData_T, URLChar_T>::Router_T&
Router<EndpointData_T, URLChar_T>::const_LNRIterator::GetRouter() const noexcept
{
	return (*m_constNativeIter).second;
};



template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::const_LNRIterator::const_LNRIterator(
	const Router<EndpointData_T, URLChar_T>::Router_T& node)
	: m_constNativeIter(GetIterFromThis(std::addressof(node)))
{
	// nothing
};



template <typename EndpointData_T, typename URLChar_T>
Router<EndpointData_T, URLChar_T>::const_LNRIterator::const_LNRIterator(
	const Router<EndpointData_T, URLChar_T>::Router_T* node)
	: m_constNativeIter(GetIterFromThis(node))
{
	// nothing
};



template <typename EndpointData_T, typename URLChar_T>
bool
Router<EndpointData_T, URLChar_T>::const_LNRIterator::HasChild() const noexcept
{
	return GetRouter().cbegin() == GetRouter().cend();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator&
Router<EndpointData_T, URLChar_T>::const_LNRIterator::operator++()
{
	if (HasChild())
	{
		m_constNativeIter = GetRouter().cbegin();
	} else
	{
		Router_T* parent = Data().Parent(); // parent always exists
		if (m_constNativeIter != parent->cend())
		{
			m_constNativeIter++;
		} else
		{
			LNRIterator parentIter = const_LNRIterator{Data().Parent()};
			*this = ++parentIter;
		}

	}

	return *this;
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_iterator
Router<EndpointData_T, URLChar_T>::const_LNRIterator::operator++(int)
{
	ConstNativeIter_T retIter = m_constNativeIter;
	
	if (HasChild())
	{
		m_constNativeIter = GetRouter().cbegin();
	} else
	{
		Router_T* parent = Data().Parent(); // parent always exists
		if (m_constNativeIter != parent->cend())
		{
			m_constNativeIter++;
		} else
		{
			LNRIterator parentIter = const_LNRIterator{Data().Parent()};
			*this = ++parentIter;
		}

	}

	return retIter;
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_LNRIterator::reference
Router<EndpointData_T, URLChar_T>::const_LNRIterator::Data() const
{
	return m_constNativeIter->second.data();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_LNRIterator::reference
Router<EndpointData_T, URLChar_T>::const_LNRIterator::operator*() const
{
	return this->Data();
};



template <typename EndpointData_T, typename URLChar_T>
typename Router<EndpointData_T, URLChar_T>::const_LNRIterator::pointer
Router<EndpointData_T, URLChar_T>::const_LNRIterator::operator->() const
{
	return std::addressof(Data());
};



template <typename EndpointData_T, typename URLChar_T>
bool
Router<EndpointData_T, URLChar_T>::const_LNRIterator::operator==(const_LNRIterator otherIter) const
{
	return m_constNativeIter == otherIter.m_constNativeIter;
};



template <typename EndpointData_T, typename URLChar_T>
bool
Router<EndpointData_T, URLChar_T>::const_LNRIterator::operator!=(const_LNRIterator otherIter) const
{
	return not this->operator==(otherIter);
};

// =================================================
// ========= end struct const_LNRIterator ==========
// =================================================