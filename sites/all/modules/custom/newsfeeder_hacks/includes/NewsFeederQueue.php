<?php

/**
 * Custom queue implementation for Newsfeeder.
 *
 * It makes sure that each queue item exists only once in the queue.
 */
class NewsFeederQueue extends SystemQueue {
  /**
   * {@inheritdoc}
   */
  public function createItem($data) {
    $serialized_data = serialize($data);
    // Make sure that, if a queue item already exists, then the existing one
    // will be updated rather than a new queue item is created.
    // Also set the expiration date of the created item to one week.
    return db_merge('queue')
      ->key(['name' => $this->name, 'data' => $serialized_data])
      ->fields([
        'name' => $this->name,
        'data' => $serialized_data,
        'created' => time(),
      ])->execute();
  }
}
