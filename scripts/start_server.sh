#!/bin/bash

AGENT="akondius@localhost"
./server &
ssh ${AGENT} sudo tail -f /var/log/osquery/osqueryd.results.log > output.log
