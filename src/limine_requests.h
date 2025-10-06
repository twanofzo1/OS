#pragma once
#include "limine.h"
#include "stdbool.h"
#include <stddef.h>

extern struct limine_framebuffer_request framebuffer_request;
extern volatile uint64_t limine_base_revision[];

bool limine_ok();