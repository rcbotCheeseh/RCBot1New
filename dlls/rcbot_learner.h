#ifndef __RCBOT_LEARNER_H__
#define __RCBOT_LEARNER_H__
#include <vector>
#include <math.h>

class IRCBotLearnerTransferFunction
{
public:
	virtual float transfer(float fNetInput) = 0;
	virtual float train(float fWeight, float fInput, float fError, float fLearnRate) = 0;
};

class IRCBotLearnerTransferFunction_Sigmoid : public IRCBotLearnerTransferFunction
{
public:
	virtual float transfer(float fNetInput)
	{
		return 1.0f / (1.0f + exp(-fNetInput));
	}
	virtual float train(float fWeight, float fInput, float fError, float fLearnRate)
	{
		return fWeight + fLearnRate * (fError * fInput);
	}
};

class RCBotLearner
{
public:

	virtual void SetInputs(std::vector<float>* inputs) = 0;

	virtual void Execute(std::vector<float> *output) = 0;

	virtual void Train(std::vector<float>* inputs, std::vector<float>* outputs) = 0;

protected:
	unsigned int m_iNumInputs;
	unsigned int m_iNumOutputs;
	std::vector<float>* m_fOutputs;
	float m_fLearnRate;
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

	void Execute(std::vector<float> *outputs)
	{
		// bias weight
		float fNetInput = m_fWeights[0];

		for (unsigned int i = 0; i < m_fInputs.size(); i++)
		{
			fNetInput += m_fWeights[i + 1] * m_fInputs[i];
		}

		outputs->push_back(m_transferFunction.transfer(fNetInput));
	}

	void Train(std::vector<float>* inputs,std::vector<float>* outputs)
	{
		float error = (*outputs)[0] - (*m_fOutputs)[0];
		// bias
		m_fWeights[0] = m_fWeights[0] + m_fLearnRate * error;

		for (unsigned int i = 1; i < m_fWeights.size(); i++)
		{
			m_fWeights[i] = m_transferFunction.train(m_fWeights[i], m_fInputs[i - 1], error, m_fLearnRate); 
		}
	}
private:
	std::vector<float> m_fInputs;
	std::vector<float> m_fWeights;
	IRCBotLearnerTransferFunction_Sigmoid m_transferFunction;

};

#endif // !__RCBOT_LEARNER_H__
