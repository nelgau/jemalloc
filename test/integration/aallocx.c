#include "test/jemalloc_test.h"

#define NTHREADS 10

void *
thd_start(void *arg)
{
  unsigned arena_ind;
  size_t sz;
  void *p;

  sz = sizeof(arena_ind);
  assert_d_eq(mallctl("arenas.extend", &arena_ind, &sz, NULL, 0), 0,
      "Error in arenas.extend");

  p = mallocx(1, MALLOCX_ARENA(arena_ind));
  assert_u_eq(arena_ind, aallocx(p, 0), "Unexpected aallocx() arena index");
  dallocx(p, 0);

  return (NULL);
}

TEST_BEGIN(test_aallocx)
{
  thd_t thds[NTHREADS];
  unsigned i;

  for (i = 0; i < NTHREADS; i++)
    thd_create(&thds[i], thd_start, NULL);

  for (i = 0; i < NTHREADS; i++)
    thd_join(thds[i], NULL);
}
TEST_END

int
main(void)
{
  return (test(
      test_aallocx));
}
