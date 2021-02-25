#ifndef __RCBOT_LEARNER_H__
#define __RCBOT_LEARNER_H__
#include <vector>

class RCBotLearner
{
public:

	virtual void SetInputs(std::vector<float>* inputs) = 0;

	virtual float Execute() = 0;

	virtual void Train(std::vector<float>* outputs) = 0;

protected:
	unsigned int m_iNumInputs;
	unsigned int m_iNumOutputs;
};

class RCBotPerceptron : RCBotLearner
{
public:
	RCBotPerceptron(unsigned int iNumInputs, unsigned int iNumOutputs)
	{
		m_iNumInputs = iNumInputs;
		m_iNumOutputs = iNumOutputs;

		for (unsigned int i = 0; i <= m_iNumInputs; i++)
			m_fWeights.push_back(0.0f); // Random Weight
	}

	void SetInputs(std::vector<float>* inputs)
	{
		m_fInputs.clear();

		for (unsigned int i = 0; i < inputs->size(); i++)
			m_fInputs.push_back((*inputs)[i]);
	}
private:
	std::vector<float> m_fInputs;
	std::vector<float> m_fWeights;

};


#endif // !__RCBOT_LEARNER_H__
