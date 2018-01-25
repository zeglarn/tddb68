#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);
bool create (void *esp);


void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  // Get syscall nr.
  uint32_t syscall_no = (uint32_t) f->esp;

  switch (syscall_no) {
    case SYS_HALT:
    power_off();
    break;

    case SYS_CREATE:
    f->eax = create(f->esp);
    break;
  }
  //printf ("system call!\n");
  //thread_exit ();
}

bool create(void *esp) {
  const char *file = (const char *) (esp + 4);
  uint32_t initial_size = *((uint32_t *) (esp + 8));
  return true;
}
