<script>
	import { elasticOut, bounceOut } from 'svelte/easing';
	import { flip } from 'svelte/animate';
	import { tweened } from 'svelte/motion';
	import { browser } from '$app/environment';

	let cell_width = $state(38);
	let window_inner_width = $state(1920);
	let cells_shown = $state(18);
	let pointerPos = $state(400);
	let tape = $state(new Array(10000).fill(1).map((_, i) => ({ id: i, value: 0 })));
	let cells = $derived(tape.slice(pointerPos - cells_shown / 2 + 1, cells_shown / 2 + pointerPos));
	let ypos = tweened(0, { duration: 40, easing: bounceOut });

	if (browser) {
		window.addEventListener('resize', () => {
			window_inner_width = window.innerWidth;
		});
		$effect(() => {
			cells_shown = Math.floor(window_inner_width / cell_width);
		});
	}
</script>

<input type="number" bind:value={cell_width} />
{cells_shown}<br />
<div
	class="button"
	on:click={() => {
		pointerPos--;
	}}
>
	&lt;
</div>
<div
	class="button"
	on:click={() => {
		pointerPos++;
	}}
>
	&gt;
</div>
<div
	class="button"
	on:click={() => {
		tape[pointerPos].value += 1;
		tape = tape;
		ypos.set(-7).then(() => ypos.set(0));
	}}
>
	+
</div>
<div
	class="button"
	on:click={() => {
		tape[pointerPos].value -= 1;
		tape = tape;
		ypos.set(7).then(() => ypos.set(0));
	}}
>
	-
</div>
<div id="tape-wrapper">
	<div
		id="tape"
		on:wheel={(e) => {
			e.preventDefault();
			return false;
		}}
	>
		{#each cells as cell, i (cell.id)}
			<div
				class="cell"
				animate:flip={{ duration: 200 }}
				style:transform={cell.id == pointerPos ? `translateY(${$ypos}px)` : ''}
				data-pos={i}
				on:click={() => (pointerPos = cell.id)}
			>
				{cell.value}
			</div>
		{/each}
	</div>
	<div id="pointer" class="gradient-border"></div>
</div>

<style>
	#tape-wrapper {
		width: calc(100vw - 4em);
		margin: auto;
		position: relative;
	}
	#tape {
		display: flex;
		flex-flow: row nowrap;
		width: max-content;
		gap: 4px;
		overflow-x: hidden;
		width: 100%;
	}
	#tape-wrapper::before {
		content: '';
		display: block;
		position: absolute;
		left: 0;
		top: 0;
		background: rgb(255, 255, 255);
		background: linear-gradient(
			90deg,
			rgba(255, 255, 255, 1) 0%,
			rgba(255, 255, 255, 1) 35%,
			rgba(255, 255, 255, 0) 100%
		);
		width: 6em;
		z-index: 200;
		height: 100%;
	}
	#tape-wrapper::after {
		content: '';
		display: block;
		position: absolute;
		right: 0;
		top: 0;
		background: rgb(255, 255, 255);
		background: linear-gradient(
			-90deg,
			rgba(255, 255, 255, 1) 0%,
			rgba(255, 255, 255, 1) 35%,
			rgba(255, 255, 255, 0) 100%
		);
		width: 6em;
		z-index: 200;
		height: 100%;
	}
	.cell {
		width: 1.9em;
		height: 2em;
		background: wheat;
		border-radius: 0.3em;
		text-align: center;
		flex-shrink: 0;
		color: black;
		display: grid;
		place-content: center;
		margin-top: 1em;
		margin-bottom: 1em;
	}

	#pointer {
		width: 2em;
		height: 4em;
		position: absolute;
		left: 50%;
		top: -0.4em;
		background: #bbbbbb33;
		box-sizing: content-box;
		z-index: 10;
		transform: translate(-50%, 1em) translate(-0.4em);
		padding: 0.1em;
		border-radius: 3px;
	}

	#pointer::after {
		content: '';
		display: block;
		position: absolute;
		bottom: -0.7em;
		width: 2em;
		height: 2em;
		border-top: 6px solid #7f27ab;
		border-left: 6px solid #7f27ab;
		border-radius: 0.1em;
		transform: translate(-2px) rotateZ(45deg) scale(0.4);
	}
	.button {
		user-select: none;
	}

	.gradient-border {
		--box-border--border: linear-gradient(105deg, rgb(255 46 144) 0%, rgb(61 35 185) 100%);

		/* classic 9-slide-scaling with 2px border 4px rounded corners. change `rx` and `ry` parameters to adjust border-radius */
		--box--border__top-left: url("data:image/svg+xml,<svg width='10' height='10' viewBox='0 0 10 10' fill='none' xmlns='http://www.w3.org/2000/svg'><rect x='1' y='1' width='18' height='18' rx='6' ry='6' stroke='%23000' stroke-width='3' /></svg>");
		--box--border__top: url("data:image/svg+xml,<svg preserveAspectRatio='none' width='100' height='10' viewBox='0 0 100 10' fill='none' xmlns='http://www.w3.org/2000/svg'><line x1='-1' y1='1' x2='101' y2='1' stroke='%23000' stroke-width='3'/></svg>");
		--box--border__top-right: url("data:image/svg+xml,<svg width='10' height='10' viewBox='0 0 10 10' fill='none' xmlns='http://www.w3.org/2000/svg'><rect x='-9' y='1' width='18' height='18' rx='6' ry='6' stroke='black' stroke-width='3' /></svg>");
		--box--border__left: url("data:image/svg+xml,<svg preserveAspectRatio='none' width='10' height='100' viewBox='0 0 10 100' fill='none' xmlns='http://www.w3.org/2000/svg'><line x1='1' y1='-1' x2='1' y2='101' stroke='%23000' stroke-width='3'/></svg>");
		--box--border__right: url("data:image/svg+xml,<svg preserveAspectRatio='none' width='10' height='100' viewBox='0 0 10 100' fill='none' xmlns='http://www.w3.org/2000/svg'><line x1='9' y1='-1' x2='9' y2='101' stroke='%23000' stroke-width='3'/></svg>");
		--box--border__bottom-left: url("data:image/svg+xml,<svg width='10' height='10' viewBox='0 0 10 10' fill='none' xmlns='http://www.w3.org/2000/svg'><rect x='1' y='-9' width='18' height='18' rx='6' ry='6' stroke='%23000' stroke-width='3' /></svg>");
		--box--border__bottom: url("data:image/svg+xml,<svg preserveAspectRatio='none' width='100' height='10' viewBox='0 0 100 10' fill='none' xmlns='http://www.w3.org/2000/svg'><line x1='-1' y1='9' x2='101' y2='9' stroke='%23000' stroke-width='3'/></svg>");
		--box--border__bottom-right: url("data:image/svg+xml,<svg width='10' height='10' viewBox='0 0 10 10' fill='none' xmlns='http://www.w3.org/2000/svg'><rect x='-9' y='-9' width='18' height='18' rx='6' ry='6' stroke='black' stroke-width='3' /></svg>");

		position: relative;
		overflow: hidden;
	}

	.gradient-border::before {
		content: '';
		position: absolute;
		top: 0;
		left: 0;
		width: 100%;
		height: 100%;
		background: var(--box-border--border);
		mask:
			var(--box--border__top-left) 0 0 / 0.625rem 0.625rem,
			var(--box--border__top) 0.625rem 0 / calc(100% - 1.25rem) 0.625rem,
			var(--box--border__top-right) 100% 0 / 0.625rem 0.625rem,
			var(--box--border__left) 0 0.625rem / 0.625rem calc(100% - 1.25rem),
			var(--box--border__right) 100% 0.625rem / 0.625rem calc(100% - 1.25rem),
			var(--box--border__bottom-left) 0 100% / 0.625rem 0.625rem,
			var(--box--border__bottom) 0.625rem 100% / calc(100% - 1.25rem) 0.625rem,
			var(--box--border__bottom-right) 100% 100% / 0.625rem 0.625rem;
		mask-repeat: no-repeat;
	}
</style>
