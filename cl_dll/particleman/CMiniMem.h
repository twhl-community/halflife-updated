#pragma once

#include <cstddef>
#include <memory_resource>
#include <vector>

class CBaseParticle;

#define TRIANGLE_FPS 30

/**
*	@brief Simple allocator that uses a chunk-based pool to serve requests.
*/
class CMiniMem
{
private:
	static inline CMiniMem* _instance = nullptr;

	std::pmr::unsynchronized_pool_resource _pool;

	std::vector<CBaseParticle*> _particles;
	std::size_t _visibleParticles = 0;

protected:
	// private constructor and destructor.
	CMiniMem() = default;
	~CMiniMem() = default;

public:
	void* Allocate(std::size_t sizeInBytes, std::size_t alignment = alignof(std::max_align_t));

	void Deallocate(void* memory, std::size_t sizeInBytes, std::size_t alignment = alignof(std::max_align_t));

	void ProcessAll(); //Processes all

	void Reset(); //clears memory, setting all particles to not used.

	void Shutdown();

	int ApplyForce(Vector vOrigin, Vector vDirection, float flRadius, float flStrength);

	static CMiniMem* Instance();

	std::size_t GetTotalParticles() { return _particles.size(); }
	std::size_t GetDrawnParticles() { return _visibleParticles; }
};
