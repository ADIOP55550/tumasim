
<script lang="ts">
  import "milligram/dist/milligram.min.css";
  import TMSim from "$lib/TM/TMSim.svelte"
</script>

<h1>TuMaSim</h1>

<form>
  <fieldset>
    <pre><code contenteditable=true></code></pre>
    
    <label for="initialState">Initial state:</label>
    <input type="text" placeholder="A" id="initialState" pattern="\w+">

    <label for="acceptingStates">Accepting states:</label>
    <input type="text" placeholder="B, C, D" id="acceptingStates">

    <button class="button">Load</button>
</fieldset>
</form>

<TMSim/>



