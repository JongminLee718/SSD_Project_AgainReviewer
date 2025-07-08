
class SSD {
public:
	bool doWriteCmd(int address, int data);
	void doReadCmd();
	int getData(int addr);
	void setData(int addr, int data);
private:
	static const int MAX_LBA = 100;
	int m_ssd[MAX_LBA];
};
