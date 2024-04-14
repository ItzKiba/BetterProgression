#pragma once
#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class ParticleManager {
    public:

    inline static CCParticleSystem* inwardParticles(int numParticles, ccColor3B color) {
            auto dict = CCDictionary::createWithContentsOfFileThreadSafe("dragEffect.plist");

            dict->setObject(CCString::create("1"), "emitterType");
            dict->setObject(CCString::create("-1"), "duration");
            dict->setObject(CCString::createWithFormat("%i", numParticles), "maxParticles");
            dict->setObject(CCString::create("1.5"), "particleLifespan");
            dict->setObject(CCString::create("0.5"), "particleLifespanVariance");
            dict->setObject(CCString::create("0"), "angle");
            dict->setObject(CCString::create("360"), "angleVariance");
            dict->setObject(CCString::create("0"), "sourcePositionx");
            dict->setObject(CCString::create("0"), "sourcePositiony");
            dict->setObject(CCString::create("0"), "sourcePositionVariancex");
            dict->setObject(CCString::create("0"), "sourcePositionVariancey");
            dict->setObject(CCString::create("250"), "maxRadius");
            dict->setObject(CCString::create("0"), "minRadius");
            dict->setObject(CCString::create("1"), "startParticleSize");
            dict->setObject(CCString::create("4"), "finishParticleSize");
            dict->setObject(CCString::create("1"), "startParticleSizeVariance");
            dict->setObject(CCString::create("250"), "speed");
            dict->setObject(CCString::create("125"), "speedVariance");
            dict->setObject(CCString::create("0"), "gravityx");
            dict->setObject(CCString::create("0"), "gravityy");
            dict->setObject(CCString::create(fmt::format("{}", color.r)), "startColorRed");
            dict->setObject(CCString::create(fmt::format("{}", color.g)), "startColorGreen");
            dict->setObject(CCString::create(fmt::format("{}", color.b)), "startColorBlue");
            dict->setObject(CCString::create("0.0"), "startColorAlpha");
            dict->setObject(CCString::create("0.3"), "startColorVarianceRed");
            dict->setObject(CCString::create("0.3"), "startColorVarianceBlue");
            dict->setObject(CCString::create("0.3"), "startColorVarianceGreen");
            dict->setObject(CCString::create("0"), "startColorVarianceAlpha");
            dict->setObject(CCString::create(fmt::format("{}", color.r)), "finishColorRed");
            dict->setObject(CCString::create(fmt::format("{}", color.g)), "finishColorGreen");
            dict->setObject(CCString::create(fmt::format("{}", color.b)), "finishColorBlue");
            dict->setObject(CCString::create("1.0"), "finishColorAlpha");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceRed");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceBlue");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceGreen");
            dict->setObject(CCString::create("0"), "finishColorVarianceAlpha");
            dict->setObject(CCString::create("0"), "rotatePerSecond");
            dict->setObject(CCString::create("0"), "rotatePerSecondVariance");
            dict->setObject(CCString::create("180"), "rotationEndVariance");

            dict->setObject(CCString::create("square.png"), "textureFileName");

            auto emitter = CCParticleSystemQuad::create();
            emitter->initWithDictionary(dict, false);
            
            return emitter;
        }

        inline static CCParticleSystem* outwardParticles(int numParticles, ccColor3B color) {
            auto dict = CCDictionary::createWithContentsOfFileThreadSafe("dragEffect.plist");

            dict->setObject(CCString::create("1"), "emitterType");
            dict->setObject(CCString::create("-1"), "duration");
            dict->setObject(CCString::createWithFormat("%i", numParticles), "maxParticles");
            dict->setObject(CCString::create("1.0"), "particleLifespan");
            dict->setObject(CCString::create("0.5"), "particleLifespanVariance");
            dict->setObject(CCString::create("0"), "angle");
            dict->setObject(CCString::create("360"), "angleVariance");
            dict->setObject(CCString::create("0"), "sourcePositionx");
            dict->setObject(CCString::create("0"), "sourcePositiony");
            dict->setObject(CCString::create("0"), "sourcePositionVariancex");
            dict->setObject(CCString::create("0"), "sourcePositionVariancey");
            dict->setObject(CCString::create("30"), "maxRadius");
            dict->setObject(CCString::create("100"), "minRadius");
            dict->setObject(CCString::create("5"), "startParticleSize");
            dict->setObject(CCString::create("1"), "finishParticleSize");
            dict->setObject(CCString::create("2"), "startParticleSizeVariance");
            dict->setObject(CCString::create("50"), "speed");
            dict->setObject(CCString::create("25"), "speedVariance");
            dict->setObject(CCString::create("0"), "gravityx");
            dict->setObject(CCString::create("0"), "gravityy");
            dict->setObject(CCString::create(fmt::format("{}", color.r)), "startColorRed");
            dict->setObject(CCString::create(fmt::format("{}", color.g)), "startColorGreen");
            dict->setObject(CCString::create(fmt::format("{}", color.b)), "startColorBlue");
            dict->setObject(CCString::create("1.0"), "startColorAlpha");
            dict->setObject(CCString::create("0.3"), "startColorVarianceRed");
            dict->setObject(CCString::create("0.3"), "startColorVarianceBlue");
            dict->setObject(CCString::create("0.3"), "startColorVarianceGreen");
            dict->setObject(CCString::create("0"), "startColorVarianceAlpha");
            dict->setObject(CCString::create(fmt::format("{}", color.r)), "finishColorRed");
            dict->setObject(CCString::create(fmt::format("{}", color.g)), "finishColorGreen");
            dict->setObject(CCString::create(fmt::format("{}", color.b)), "finishColorBlue");
            dict->setObject(CCString::create("0.0"), "finishColorAlpha");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceRed");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceBlue");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceGreen");
            dict->setObject(CCString::create("0"), "finishColorVarianceAlpha");
            dict->setObject(CCString::create("0"), "rotatePerSecond");
            dict->setObject(CCString::create("0"), "rotatePerSecondVariance");
            dict->setObject(CCString::create("180"), "rotationEndVariance");

            dict->setObject(CCString::create("square.png"), "textureFileName");

            auto emitter = CCParticleSystemQuad::create();
            emitter->initWithDictionary(dict, false);
            
            return emitter;
        }

        inline static CCParticleSystem* bottomParticles(int numParticles, ccColor3B color) {
            auto dict = CCDictionary::createWithContentsOfFileThreadSafe("dragEffect.plist");

            dict->setObject(CCString::create("0"), "emitterType");
            dict->setObject(CCString::create("-1"), "duration");
            dict->setObject(CCString::createWithFormat("%i", numParticles), "maxParticles");
            dict->setObject(CCString::create("2.0"), "particleLifespan");
            dict->setObject(CCString::create("0.8"), "particleLifespanVariance");
            dict->setObject(CCString::create("90"), "angle");
            dict->setObject(CCString::create("20"), "angleVariance");
            dict->setObject(CCString::create("0"), "sourcePositionx");
            dict->setObject(CCString::create("0"), "sourcePositiony");
            dict->setObject(CCString::create("300"), "sourcePositionVariancex");
            dict->setObject(CCString::create("0"), "sourcePositionVariancey");
            dict->setObject(CCString::create("4"), "startParticleSize");
            dict->setObject(CCString::create("1"), "finishParticleSize");
            dict->setObject(CCString::create("2"), "startParticleSizeVariance");
            dict->setObject(CCString::create("50"), "speed");
            dict->setObject(CCString::create("25"), "speedVariance");
            dict->setObject(CCString::create("0"), "gravityx");
            dict->setObject(CCString::create("0"), "gravityy");
            dict->setObject(CCString::create(fmt::format("{}", color.r)), "startColorRed");
            dict->setObject(CCString::create(fmt::format("{}", color.g)), "startColorGreen");
            dict->setObject(CCString::create(fmt::format("{}", color.b)), "startColorBlue");
            dict->setObject(CCString::create("1.0"), "startColorAlpha");
            dict->setObject(CCString::create("0.3"), "startColorVarianceRed");
            dict->setObject(CCString::create("0.3"), "startColorVarianceBlue");
            dict->setObject(CCString::create("0.3"), "startColorVarianceGreen");
            dict->setObject(CCString::create("0"), "startColorVarianceAlpha");
            dict->setObject(CCString::create(fmt::format("{}", color.r)), "finishColorRed");
            dict->setObject(CCString::create(fmt::format("{}", color.g)), "finishColorGreen");
            dict->setObject(CCString::create(fmt::format("{}", color.b)), "finishColorBlue");
            dict->setObject(CCString::create("0.0"), "finishColorAlpha");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceRed");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceBlue");
            dict->setObject(CCString::create("0.1"), "finishColorVarianceGreen");
            dict->setObject(CCString::create("0"), "finishColorVarianceAlpha");
            dict->setObject(CCString::create("0"), "rotatePerSecond");
            dict->setObject(CCString::create("0"), "rotatePerSecondVariance");
            dict->setObject(CCString::create("180"), "rotationEndVariance");

            dict->setObject(CCString::create("square.png"), "textureFileName");

            auto emitter = CCParticleSystemQuad::create();
            emitter->initWithDictionary(dict, false);
            
            return emitter;
        }
};