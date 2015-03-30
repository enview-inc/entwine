/******************************************************************************
* Copyright (c) 2016, Connor Manning (connor@hobu.co)
*
* Entwine -- Point cloud indexing
*
* Entwine is available under the terms of the LGPL2 license. See COPYING
* for specific license text and more information.
*
******************************************************************************/

#pragma once

#include <cstddef>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include <entwine/tree/roller.hpp>
#include <entwine/types/elastic-atomic.hpp>
#include <entwine/util/file-descriptor.hpp>

namespace entwine
{

class Branch;
class Clipper;
class PointInfo;

namespace fs
{

// Currently hard-coded to map the file in sections of fileSize / 256.
class PointMapper
{
public:
    PointMapper(
            const Schema& schema,
            const std::string& filename,
            std::size_t fileSize,
            std::size_t firstPoint);
    ~PointMapper();

    bool addPoint(PointInfo** toAddPtr, const Roller& roller);
    Point getPoint(std::size_t index);
    std::vector<char> getPointData(std::size_t index);

    void clip(Clipper* clipper, std::size_t slotIndex);

    char* ensureMapping(Clipper* clipper, std::size_t index);

private:
    char* getMapping(std::size_t slotIndex) const;

    std::size_t getSlotIndex(std::size_t index) const;
    std::size_t getSlotOffset(std::size_t index) const;
    std::size_t getGlobalOffset(std::size_t index) const;

    const Schema& m_schema;
    FileDescriptor m_fd;

    const std::size_t m_slotSize;

    const std::size_t m_firstPointIndex;

    // TODO Wrap these.
    std::vector<ElasticAtomic<char*>> m_mappings;
    std::vector<std::mutex> m_locks;
    std::vector<std::set<Clipper*>> m_slotRefs;
};

} // namespace fs
} // namespace entwine

