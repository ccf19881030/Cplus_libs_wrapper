#pragma once

class IProgramService
{
public:
	virtual void run() = 0;
	virtual void stop() = 0;
};
