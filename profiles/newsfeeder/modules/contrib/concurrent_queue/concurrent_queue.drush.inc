<?php

/**
 * @file
 * Drush integration for the Concurrent Queue module.
 */

/**
 * Implements hook_drush_command_alter().
 */
function concurrent_queue_drush_command_alter(&$command) {
  if ($command['command'] == 'queue-run') {
    $command['callback'] = 'drush_concurrent_queue_queue_run_concurrent';
  }
}

/**
 * Implements hook_drush_help_alter().
 */
function concurrent_queue_drush_help_alter(&$command) {
  if ($command['command'] == 'queue-run') {
    $command['options']['concurrency'] = dt('The number of parallel processes to run. The default concurrency is 1.');
    $command['options']['override-time-limit'] = dt('Override the time limit set in hook_cron_queue_info() when processing. Time in seconds.');
    $command['options']['no-reclaim'] = dt('Will not attempt to reclaim items from the queue before processing.');
    $command['aliases'] = array('qr');
    $command['examples']['drush qr example_queue --concurrency=5'] = 'drush qr example_queue --concurrency=5';
  }
}

/**
 * Implements hook_drush_command().
 */
function concurrent_queue_drush_command() {
  $items['queue-run-all'] = array(
    'description' => 'Runs all queues.',
    'options' => array(
      'concurrency' => 'The number of parallel processes to run. The default concurrency is 1.',
      'queue-concurrency' => 'The number of parallel processes to use when running an individual queue. The default is 1.',
      'override-time-limit' => 'Override the time limit set in hook_cron_queue_info() when processing. Time in seconds.',
    ),
  );

  $items['queue-run-concurrent'] = array(
    'description' => 'Concurrently run a specific queue by name',
    'arguments' => array(
      'queue_name' => 'The name of the queue to run, as defined in either hook_queue_info or hook_cron_queue_info.',
    ),
    'options' => array(
      'concurrency' => 'The number of parallel processes to run. The default concurrency is 1.',
      'override-time-limit' => 'Override the time limit set in hook_cron_queue_info() when processing. Time in seconds.',
      'no-reclaim' => 'Will not attempt to reclaim items from the queue before processing.',
    ),
    'aliases' => array('qrc'),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush queue-run-concurrent example_queue --concurrency=5 --override-time-limit=300' => "Run the 'example_queue' queue with five different processes, each running for up to 5 minutes.",
    ),
    'hidden' => TRUE,
  );

  return $items;
}

/**
 * Command callback for drush queue-run-all.
 */
function drush_concurrent_queue_queue_run_all() {
  $concurrency = (int) drush_get_option('concurrency', '1');

  $queues = DrushConcurrentQueue::getAllQueueInfo();
  $invocations = array();
  foreach (array_keys($queues) as $queue_name) {
    $invocations[] = array(
      'site' => '@self',
      'command' => 'queue-run',
      'args' => array($queue_name),
    );
  }

  $commandline_options = array();
  foreach (array('override-time-limit', 'queue-concurrency') as $option) {
    $value = drush_get_option($option);
    if (isset($value)) {
      $commandline_options[$option] = $value;
    }
  }
  $backend_options = array(
    'concurrency' => $concurrency,
  );

  return drush_backend_invoke_concurrent($invocations, $commandline_options, $backend_options);
}

/**
 * Validation callback for drush queue-run.
 */
function drush_concurrent_queue_queue_run_validate($queue_name) {
  try {
    DrushConcurrentQueue::getInstance($queue_name);
  }
  catch (Exception $e) {
    drush_set_error('DRUSH_QUEUE_VALIDATION_EXCEPTION', $e->getMessage());
  }
}

/**
 * Validation callback for drush queue-run-concurrent.
 */
function drush_concurrent_queue_queue_run_concurrent_validate($queue_name) {
  drush_concurrent_queue_queue_run_validate($queue_name);
}

/**
 * Command callback for drush queue-run and queue-run-concurrent.
 */
function drush_concurrent_queue_queue_run_concurrent($queue_name) {
  $instance = DrushConcurrentQueue::getInstance($queue_name);

  // Reset expired items in process queues.
  if (!drush_get_option('no-reclaim') && $reclaim_count = $instance->reclaimItems()) {
    drush_log(dt("Reset @count items that had expired in the @queue queue.", array('@count' => $reclaim_count, '@queue' => $queue_name)), 'ok');
  }

  $count_before = $instance->queue->numberOfItems();
  if (!$count_before) {
    drush_log(dt("There are no items to process in the @name queue.", array('@name' => $queue_name)), 'ok');
    return;
  }

  $concurrency = (int) drush_get_option('concurrency', 1);
  if ($concurrency > 1) {
    $concurrency = min($concurrency, $count_before);
    $instance->invokeWorkers($concurrency);
  }
  else {
    $instance->run();
  }
}

class DrushConcurrentQueue {

  /**
   * @var DrupalReliableQueueInterface
   */
  public $queue;

  public function __construct($queue_name, $time_limit = NULL) {
    $this->name = $queue_name;
    $this->info = static::getQueueInfo($queue_name);
    if (!$this->info) {
      throw new Exception("Unable to find info for queue $queue_name.");
    }
    $this->callback = $this->info['worker callback'];
    if (!is_callable($this->callback)) {
      throw new Exception(dt('Unable to find worker callback @callback for the @queue queue.', array('@callback' => $this->callback, '@queue' => $queue_name)));
    }
    $this->queue = DrupalQueue::get($queue_name, TRUE);

    $this->startTime = NULL;
    $this->timeLimit = $time_limit;
  }

  /**
   * @return static
   */
  public static function getInstance($queue_name) {
    static $instances = array();
    if (!isset($instances[$queue_name])) {
      $instances[$queue_name] = new static($queue_name);
    }
    return $instances[$queue_name];
  }

  public function __call($name, $arguments) {
    return call_user_func_array(array($this->queue, $name), $arguments);
  }

  /**
   * @return array
   */
  public static function getAllQueueInfo() {
    $queues = drupal_static(__METHOD__);
    if (!isset($queues)) {
      // Invoke hook_queue_info(), currently only defined by the queue_ui module.
      $queues = module_invoke_all('cron_queue_info');
      drupal_alter('cron_queue_info', $queues);

      // Merge in queues from the queue_ui.module's hook_queue_info().
      foreach (module_invoke_all('queue_info') as $queue_name => $queue) {
        $queues[$queue_name] = array(
          'worker callback' => $queue['cron']['callback'],
        );
      }
    }
    return $queues;
  }

  /**
   * @param $queue_name
   * @return array|bool
   */
  public static function getQueueInfo($queue_name) {
    $queues = static::getAllQueueInfo();
    return isset($queues[$queue_name]) ? $queues[$queue_name] : FALSE;
  }

  public function reclaimItems() {
    if (method_exists($this->queue, 'reclaimItems')) {
      return $this->queue->reclaimItems();
    }
    elseif (is_a($this->queue, 'SystemQueue')) {
      // If a parent queue is being run, reclaim all un-run and expired items
      // assigned to subqueues or the parent queue.
      $query = db_update('queue');
      $query->fields(array('expire' => 0, 'name' => $this->name));
      $expire_condition = db_or();
      $expire_condition->condition('expire', 0);
      $expire_condition->condition('expire', time(), '<');
      $query->condition($expire_condition);
      $name_condition = db_or();
      $name_condition->condition('name', $this->name);
      // Deprecated support for when the subqueues had different names.
      $name_condition->condition('name', db_like($this->name . ':') . '%', 'LIKE');
      $query->condition($name_condition);
      return $query->execute();
    }
    else {
      drush_log(dt("Unable to reclaim items for queue $this->name since it does not use or extend the SystemQueue class."), 'warning');
      return FALSE;
    }
  }

  public function getTimeLimit($set = TRUE) {
    $max_execution_time = ini_get('max_execution_time');

    if (!isset($this->timeLimit)) {
      if (!$max_execution_time) {
        // If there is no limit to the max execution time, then set a more
        // reasonable limit of one day running time.
        $this->timeLimit = 86400;
      }
      else {
        $this->timeLimit = (isset($this->info['cron']['time']) ? $this->info['cron']['time'] : 60);
      }
    }

    // Try to increase the maximum execution time if it is too low.
    if ($set && $max_execution_time > 0 && $max_execution_time < ($this->timeLimit + 30) && !ini_get('safe_mode')) {
      drush_log(dt("Calling set_time_limit(@value).", array('@value' => $this->timeLimit + 30)), 'ok');
      @set_time_limit($this->timeLimit + 30);
    }

    return $this->timeLimit;
  }

  public function startTimer() {
    $this->startTime = microtime(TRUE);
  }

  public function readTimer() {
    return (microtime(TRUE) - $this->startTime);
  }

  /**
   * Spin off a specific number of concurrent workers to run queues.
   *
   * @param int $concurrency
   *   The number of worker queues to start.
   *
   * @return int
   *   The number of queue items processed by the worker queues.
   */
  public function invokeWorkers($concurrency) {
    $invocations = array_fill(0, $concurrency, array(
      'site' => '@self',
      'command' => 'queue-run',
      'args' => array($this->name),
    ));

    $commandline_options = array(
      'concurrency' => 1,
      'no-reclaim' => TRUE,
      // Internal flag to indicate this is a worker queue.
      'strict' => 0,
      'is-worker' => TRUE,
    );
    foreach (array('override-time-limit') as $option) {
      $value = drush_get_option($option);
      if (isset($value)) {
        $commandline_options[$option] = $value;
      }
    }
    $backend_options = array(
      'concurrency' => count($invocations),
    );

    drush_log(dt("Starting @concurrency queue @name workers. There are @count items in the queue.", array(
      '@concurrency' => $concurrency,
      '@name' => $this->name,
      '@count' => $this->queue->numberOfItems()
    )), 'ok');

    $this->getTimeLimit();

    $this->startTimer();
    $results = drush_backend_invoke_concurrent($invocations, $commandline_options, $backend_options);
    $elapsed = $this->readTimer();

    // Calculate the number of items actually processed using the results from
    // the drush_backend_invoke_concurrent() call.
    $count = 0;
    if (!empty($results['concurrent'])) {
      foreach ($results['concurrent'] as $result) {
        foreach ($result['log'] as $log) {
          if (preg_match('/Processed (\d+) items from queue ' . preg_quote($this->name, '/') . '/', $log['message'], $matches)) {
            $count += $matches[1];
          }
        }
      }
    }

    // Force the database connection to be reconnected since the workers may
    // have run for an extended amount of time.
    Database::closeConnection();

    drush_log(
      dt("Concurrently processed @count items from queue @queue in @elapsed sec (@rate/min). There are @remaining remaining items.",
        array(
          '@count' => $count,
          '@queue' => $this->name,
          '@elapsed' => round($elapsed, 2),
          '@rate' => round(($count / $elapsed) * 60),
          '@remaining' => $this->queue->numberOfItems(),
        )
      ),
      drush_get_error() ? 'warning' : 'ok'
    );

    return $count;
  }

  /**
   * Run a queue process for as long as possible.
   *
   * @return int
   *   The number of items successfully processed from the queue.
   */
  public function run() {
    $function = $this->info['worker callback'];
    if ($limit = drush_get_option('override-time-limit')) {
      $this->timeLimit = $limit;
    }
    $limit = $this->getTimeLimit();
    $end = time() + $limit;
    $count = 0;
    $lease_time = drush_get_option('lease-time', 30);

    drush_log(dt("Running the queue @queue for a maximum of @time or until empty. There are @count items in the queue.", array(
      '@queue' => $this->name,
      '@time' => format_interval($limit),
      '@count' => $this->queue->numberOfItems(),
    )), 'info');

    $this->startTimer();
    while (time() < $end && $item = $this->queue->claimItem($lease_time)) {
      try {
        drush_log(dt("Processing item @id from @queue.", array('@id' => $item->item_id, '@queue' => $this->name)), 'info');
        $function($item->data);
        $this->queue->deleteItem($item);
        $count++;
      }
      catch (Exception $e) {
        watchdog_exception($this->info['queue_name'], $e);
        drush_set_error('QUEUE_EXCEPTION', $e->getMessage());
      }
    }
    $elapsed = $this->readTimer();

    if (drush_get_option('is-worker')) {
      drush_log(
        dt("Processed @count items from queue @queue in @elapsed sec (@rate/min).",
          array(
            '@count' => $count,
            '@queue' => $this->name,
            '@elapsed' => round($elapsed, 2),
            '@rate' => round(($count / $elapsed) * 60),
          )
        ),
        drush_get_error() ? 'warning' : 'info'
      );
    }
    else {
      drush_log(
        dt("Processed @count items from queue @queue in @elapsed sec (@rate/min). There are @remaining remaining items.",
          array(
            '@count' => $count,
            '@queue' => $this->name,
            '@elapsed' => round($elapsed, 2),
            '@rate' => round(($count / $elapsed) * 60),
            '@remaining' => $this->queue->numberOfItems(),
          )
        ),
        drush_get_error() ? 'warning' : 'ok'
      );
    }

    return $count;
  }
}
