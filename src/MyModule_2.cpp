#include "plugin.hpp"


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

	MyModule_2() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(OVERDRIVE_AMOUNT_PARAM, 0.f, 1.f, 0.f, "");
		configInput(AUDIO_IN_INPUT, "");
		configOutput(AUDIO_OUT_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		float out = 0.f;
		float in = inputs[AUDIO_IN_INPUT].getVoltage();
		if (in >= 0.f) {
			out = 4.f;
		}
		else {
			out = -4.f;
		}
		//float out = in * params[OVERDRIVE_AMOUNT_PARAM].getValue();
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