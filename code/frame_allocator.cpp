

uint8_t* frame_allocator_memory;
size_t frame_allocator_used;
size_t frame_allocator_capacity;

// int* array
// array[6]
// *(array + 6)
// 6[array]
// *(6 + array)

void frame_allocator_init() {
	//load capacity
	frame_allocator_capacity = 1000;
	frame_allocator_memory = (uint8_t*)malloc(frame_allocator_capacity);
	frame_allocator_used = 0;
}

void* frame_alloc(size_t size) {
	//verif qu on ne vas pas plus loin que la memory max alloué dans init
	if (frame_allocator_used + size < frame_allocator_capacity) {
		uint8_t* buffer = frame_allocator_memory + frame_allocator_used; //uint8_t* buffer = &frame_allocator_memory[frame_allocator_used];
		frame_allocator_used += size;
		return (void*) buffer; 
	}
	else {
		printf("OUT OF MEMORY");
		return 0; 
	}
}

void frame_allocator_tick() {
	//reset le frame allocator a la place initiale 
	frame_allocator_used = 0;
}