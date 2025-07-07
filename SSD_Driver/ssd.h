#include <string>

class SSD {
public:
	void doWriteCmd();
	void doReadCmd(const std:: string& command_type, int lba);
private:
	int m_ssd[100];
};
