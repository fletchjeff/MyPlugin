#include "plugin.hpp"
#include "daisy/daisysp.h"
#include "daisy/Effects/overdrive.h"

// using namespace daisysp;

struct MyModule_2 : Module {
	enum ParamId {
		OVERDRIVE_AMOUNT_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		AUDIO_IN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		AUDIO_OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	daisysp::Overdrive od;

	//double time = 0.0;
	//dsp::BooleanTrigger resetParamTrigger;
	//dsp::ClockDivider lightDivider;
	// uint32_t clock_out; 
	bool audio_state = false;
	bool audio_state_prev = false;
	bool sub_state = false;
	bool sub_state_prev = false;
	// bool flipFlop(bool input, bool &state) {
	// 	bool output = state;
	// 	state = input;
	// 	return output;
	// }

	MyModule_2() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(OVERDRIVE_AMOUNT_PARAM, 0.f, 1.f, 0.f, "");
		configInput(AUDIO_IN_INPUT, "");
		configOutput(AUDIO_OUT_OUTPUT, "");
		od.Init();
		//lightDivider.setDivision(2);
	}

	void process(const ProcessArgs& args) override {
		float out = 0.f;
		float sub_out = 0.f;
		float in = inputs[AUDIO_IN_INPUT].getVoltage();

		// // creates the square wave out. `audio_state` is remains true while the input is greater than 0
		// if (in > 0) {
		// 	out = 4.f;
		// 	audio_state = true;
		// } else {
		// 	out = -4.f;
		// 	audio_state = false;
		// }

		// // this creates the a clock divided output of the square wave. `sub_state_prev` is toggled based
		// // on `audio_state_prev`, `audio_state` and `sub_state_prev`. This creates a clock divided output of bools
		// if (audio_state_prev == false && audio_state == true && sub_state_prev == false) {
		// 	sub_state = true;
		// } else if (audio_state_prev == false && audio_state == true && sub_state_prev == true) {
		// 	sub_state = false;
		// }

		// // this now creates a square wave output from the clock divided output which is effetively an octave
		// // lower.
		// if (sub_state) {
		// 	sub_out = 4.f;
		// } else {
		// 	sub_out = -4.f;
		// }

		// // the `audio_state_prev` and `sub_state_prev` need to be updated at the end of the process.
		// audio_state_prev = audio_state;
		// sub_state_prev = sub_state;

		// this is the overdrive effect
		od.SetDrive(params[OVERDRIVE_AMOUNT_PARAM].getValue());

		// if (params[OVERDRIVE_AMOUNT_PARAM].getValue() > 0.5) {
		// 	outputs[AUDIO_OUT_OUTPUT].setVoltage(out);
		// } else {
		// 	outputs[AUDIO_OUT_OUTPUT].setVoltage(sub_out);
		// }
		
		out = od.Process(in);
		outputs[AUDIO_OUT_OUTPUT].setVoltage(out);
	}
};


struct MyModule_2Widget : ModuleWidget {
	MyModule_2Widget(MyModule_2* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/MyModule_2.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(40.196, 32.7)), module, MyModule_2::OVERDRIVE_AMOUNT_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.171, 64.25)), module, MyModule_2::AUDIO_IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(69.228, 64.25)), module, MyModule_2::AUDIO_OUT_OUTPUT));
	}
};


Model* modelMyModule_2 = createModel<MyModule_2, MyModule_2Widget>("MyModule_2");