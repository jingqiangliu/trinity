/*
 * SYSCALL_DEFINE2(stat64, const char __user *, filename,
                 struct stat64 __user *, statbuf)
 */
{
	.name = "stat64",
	.num_args = 2,
	.arg1name = "filename",
	.arg1type = ARG_ADDRESS,
	.arg2name = "statbuf",
	.arg2type = ARG_ADDRESS,
},
