#include "core/rendering/DescriptorPool.h"

DescriptorPool::DescriptorPool()
{

}

DescriptorPool::~DescriptorPool()
{

}

void DescriptorPool::clean()
{

}

void DescriptorPool::setData(const DescriptorPoolCreateInfo& createInfo)
{
    m_info = createInfo;
}

int DescriptorPool::createDescriptorPool()
{
    return 0;
}

int DescriptorPool::createDescriptorPool(DescriptorPool* descriptorPool, const DescriptorPoolCreateInfo& createInfo)
{
    descriptorPool->setData(createInfo);

    return descriptorPool->createDescriptorPool();
}

DescriptorPool::DescriptorPoolCreateInfo& DescriptorPool::getInfo()
{
    return m_info;
}
