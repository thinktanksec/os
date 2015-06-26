# os

The Athena OS was developed for the Altera Media computer. It is a small operating system that fully implements context switching and other core os functions. Designed to run user "programs" which are just long functions that the OS will switch from when its internal timer runs out (mili seconds).

In order to run the OS on your PC, you would need to implement a way for it to get at an internal timing source to trigger the context switch. That and remove some of the Altera Macros.
