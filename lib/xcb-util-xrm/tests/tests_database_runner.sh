#!/bin/sh
command -v xvfb-run > /dev/null || exit 77 # Skip test if no xvfb-run in $PATH
exec xvfb-run tests/tests_database
