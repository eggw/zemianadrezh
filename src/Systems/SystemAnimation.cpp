#include "SystemAnimation.h"

#include "Components/ComponentAnimation.h"
#include "Components/ComponentDirection.h"
#include "Components/ComponentPhysics.h"
#include "Components/ComponentSprite.h"
#include "Components/ComponentPosition.h"

SystemAnimation::SystemAnimation() {
}

//Iterate through all entites with
void SystemAnimation::update(int _timeslice, entt::registry& _reg) {
	auto view = _reg.view<ComponentAnimation,
	                      ComponentDirection,
	                      ComponentPhysics,
	                      ComponentPosition>();
	for (auto& entity : view) {
		auto&      animation = view.get<ComponentAnimation>(entity).m_animation;
		const auto left      = view.get<ComponentDirection>(entity).m_facingLeft;
		const auto vel       = view.get<ComponentPhysics>(entity).m_velocity;

		if (left) {
			animation.setFacingLeft();
		}
		else {
			animation.setFacingRight();
		}

		float effectiveSpeed{vel.x};
		if (effectiveSpeed < 0) {
			effectiveSpeed *= -1;
		}

		animation.setAnimationSpeed(effectiveSpeed / 10);
		animation.update();
	}
}
